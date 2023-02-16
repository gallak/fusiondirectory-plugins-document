<?php
/*
  This code is part of FusionDirectory (http://www.fusiondirectory.org/)
  Copyright (C) 2016-2017  FusionDirectory
  Copyright (C) 2021  Antoine Gallavardin

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 */



 class templateSelectAttribute extends SelectAttribute
 {


    function getCurrentTemplates($parent_type) : array{
        global $config;
        $allCurrentTemplates=array();   
        $allTemplates=$config->get_cfg_value('DocumentList', "");
        if (! empty($allTemplates)){
          foreach($allTemplates as $doc){
            if (preg_match("/^$parent_type/",$doc)){
                $templateValues=preg_split("/;/",$doc);
                $allCurrentTemplates[$templateValues[2]." ("._($templateValues[3]).")"]=$doc;
            }
          }
          return($allCurrentTemplates);
        }else{
          return(array());
        }
      }


     function setParent (&$plugin)
     {
        // initial object
        $parent_type=strtolower($plugin->parent->objectType);
        parent::setParent($plugin);
         if (is_object($this->plugin)) {
             $this->updateChoices($parent_type);
         }
     }
 
     function updateChoices ($parent_type)
     {
         $templateData    = [];
         $templateDescription = [];
         foreach ($this->getCurrentTemplates($parent_type) as $content){
             $templateValues=preg_split("/;/",$content);
             $templateDescription[] = $templateValues[2]." ("._($templateValues[3]).")";
             $templateData[] = $content;
             $this->setChoices($templateData, $templateDescription);
         }
     }
 }
 

class document extends simplePlugin
{

  protected $displayHeader  = FALSE;
  protected $loaded = FALSE;
  

  static function plInfo (): array
  {
    return [
      'plShortName'   => _('Document'),
      'plDescription' => _('Document generator'),
      'plPriority'    => 30,
      'plObjectType'  => ['user','server'],
      'plProvidedAcls'  => parent::generatePlProvidedAcls(static::getAttributesInfo())
    ];
  }
  // return working directory 
  function getFdTempDir() : string{
    global $config;
    return($config->get_cfg_value('DocumentPrintFolder', "/var/cache/fusiondirectory/tmp"));
  } 

  // launching action ( print or download)
  public function launchAction (){
    global $config;

    $actionInfos=preg_split("/;/",$this->fakeOutputAttribute);

    switch ($actionInfos[3]) {
        case "print" :
            $printCommand=$config->get_cfg_value('DocumentPrintCommand', "lpr %s %s");

            $output = $actionInfos[4];
            break;
        case "download" : 
            $printCommand="";
            $output = $actionInfos[4].".pdf";
            break;

    }
    $doc=new pdfBuilder(
        $this->attrs,
        CONFIG_DIR."document/".$actionInfos[1],
        $this->getFdTempDir(),
        array("type" => "$actionInfos[3]", "name" => $output), $printCommand);
    $doc->generatePDF();

  }




  static function getAttributesInfo (): array
  {
    global $config;

    return [
      'notes' => [
        'name'  => _('Document'),
        'class' => ['fullwidth'],
        'attrs' => [
            new templateSelectAttribute(
                _('Output request'), _('Output required by user'),
                'fakeOutputAttribute'
              ),
            new ButtonAttribute(
                _('Generate file'), _('Generate PDF by file or printinf'),
                'fdGenerateFile', _('Generate PDF'),
                'launchAction'
              ),

        ]
      ],
    ];
  }

  function __construct ($dn = NULL, $object = NULL, $parent = NULL, $mainTab = FALSE)
  {
    parent::__construct($dn, $object, $parent, $mainTab);
    $this->attributesAccess['fakeOutputAttribute']->setInLdap(FALSE);
  }

}
