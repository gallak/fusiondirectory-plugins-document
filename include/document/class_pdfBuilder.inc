<?php


require_once('fpdf.php');

class pdfBuilder {
  private $data=array();
  private $template="default.tpl";
  private $output=array("type"=>"file", "name" => "output.pdf");
  private $tmpdir= "";
  private $printCommand="";
  
  private $pdfObject='';

  public function __construct($data, $template, $tmpdir, $output,$printCommand){
    $this->data=$data;
    $this->template=$template;
    $this->output=$output;
    $this->tmpdir=$tmpdir;
    $this->printCommand=$printCommand;
  }

  function file_get_contents_utf8($fn) {
    $content = file_get_contents($fn);
    return mb_convert_encoding($content, 'UTF-8',
           mb_detect_encoding($content, 'UTF-8, ISO-8859-1', true));
  }

  function file_get_contents_iso($fn) {
    $content = file_get_contents($fn);
    return mb_convert_encoding($content, 'ISO-8859-1',
           mb_detect_encoding($content, 'ISO-8859-1, UTF-8', true));
  }


  function getFirstValue($value){
    if (is_array($value)){
      return($value[0]);
    }else{
      return($value);
    }
  } 

  function buildDocument(){
    // import template
    $template_yaml = yaml_parse_file($this->template);
    // define document properties
    $document=$template_yaml['document'];
    
    // create PDF object
    $this->pdfObject = new FPDF($document['orientation'],
        'mm',
        array($document['size']['X'],$document['size']['Y']));
    // define break page or not
    $this->pdfObject->SetAutoPageBreak($document['breakPage']);
    // define initial font
    $this->pdfObject->SetFont(  $document['font']['name'],
        $document['font']['style'],
        $document['font']['size']);
    // walk across all page define    
    foreach ($template_yaml['body'] as $page ){
      // add page séparation
      $this->pdfObject->AddPage();
      // for each element inside page ( image text)
      foreach ($page as $element ){
        switch ($element['type']) {
        // if type is image
          case "image":
            switch($element['content']['source']){
              // source is an variable from data
              case "variable" :
                if (isset($this->data[$element['content']['value']])){
					$temp_image_file = tempnam($this->tmpdir, 'fd-');
					file_put_contents($temp_image_file,$this->getFirstValue($this->data[$element['content']['value']]));
					$this->pdfObject->Image($temp_image_file, $element['coord']['X'], $element['coord']['Y'],$element['size']['height'],$element['size']['width'],"JPG");
					unlink($temp_image_file);
				}
                break;
              // source a static file
              default : 
				if (file_exists($element['content']['value'])){
					$this->pdfObject->Image($element['content']['value'], $element['coord']['X'], $element['coord']['Y'],$element['size']['height'],$element['size']['width']);
				}
                break;
            }
          break;
                            
          // if type is text
          case "text":
          // set focus at the right position
          $this->pdfObject->SetXY($element['coord']['X'],$element['coord']['Y']);
          // define new style if defined
          if (isset($element['font'])){
            $this->pdfObject->SetFont(  $element['font']['name'],
            $element['font']['style'],
            $element['font']['size']);
          }
          // fetch text ton insert
          switch($element['content']['source']){
            // source is an variable from data
            case "variable" : 
              $value =  $this->getFirstValue($this->data[$element['content']['value']]);
              //var_dump($value);              
              break;
            // source is a texte from file
            case "file" : 
              // source is a static text from template
              $value = $this->file_get_contents_iso($element['content']['value']);
              break;
            default :
              $value = $element['content']['value'];
              break;
            }
          // insert txt inside PDF
          $this->pdfObject->MultiCell($element['style']['width'],
              $element['style']['interline'],
              $value,
              $element['style']['border'],
              $element['style']['alignment']);        
          break;
        default:
          break;
        }
      }
    
    }
  }


  public function printDocument($fileName, $printerName = 'default'){
    $cmd=sprintf($this->printCommand,$printerName,$fileName);
    exec($cmd,$out);
    msg_dialog::display(_('Result of print command'), $out[0] ,INFO_DIALOG);
    }




  function generatePDF(){
  
    $this->buildDocument();
   
    switch($this->output['type']){
		// source is an variable from data
    case "file" :
			try{
				$this->pdfObject->Output('F', $this->output['name']);
			}catch(Exception $ex){
        msg_dialog::display(_('Error while file generation'), $ex->getMessage() ,INFO_DIALOG);
			}
			break;
      
		case "download" :
      $full_file_path=$this->tmpdir."/".$this->output['name'];
			$this->pdfObject->Output('F', $full_file_path);
      $handle = fopen($full_file_path,"r");
      $content_file = fread($handle, filesize($full_file_path));
      send_binary_content($content_file, $this->output['name']);
			break;
		case "print" :

      $full_file_path=tempnam($this->tmpdir,"prt-");
      $this->pdfObject->Output('F', $full_file_path);
      // change permission
      chmod($full_file_path,0755);
			$this->printDocument($full_file_path,$this->output['name']);
			break;

    }
    unlink($full_file_path);
  }
  
}
