document:
  size: {"X": "86", "Y": "54"}
  orientation: "L"
  font: {"name": "Arial", "style": "B", "size": "7"}
  breakPage: false
body:
  page_1 :
    element_1 :
      type: image
      coord : {"X" : "62","Y" : "9"}
      content: {"source" : "variable", "value" : "jpegPhoto"}
      size : {"height": "20","width":"0"}

    element_2 :
      type: text
      coord : {"X" : "20","Y" : "12"}
      font: {"name": "Arial", "style": "B", "size": "7"}
      content: {"source":"variable","value":"displayName"}
      style: {"width":"40","interline":"3","border":"0","alignment":"R"}

    element_3 :
      type: text
      coord : {"X" : "20","Y" : "15"}
      font: {"name": "Arial", "style": "B", "size": "7"}
      content: {"source":"variable","value":"mail"}
      style: {"width":"40","interline":"3","border":"0","alignment":"R"}

    element_4 :
      type: text
      coord : {"X" : "20","Y" : "18"}
      font: {"name": "Arial", "style": "B", "size": "7"}
      content: {"source":"variable","value":"st"}
      style: {"width":"40","interline":"3","border":"0","alignment":"R"}


    element_5 :
      type: text
      coord : {"X" : "20","Y" : "21"}
      font: {"name": "Arial", "style": "B", "size": "6"}
      content: {"source":"variable","value":"mail"}
      style: {"width":"40","interline":"3","border":"0","alignment":"R"}

  page_2 :
    element_1 :
      type: image
      coord : {"X" : "35","Y" : "15"}
      content: {"source" : "file", "value" : "/etc/fusiondirectory/document/webservice.png"}
      size : {"height": "16","width":"0"}
      
    element_2 :
      type: text
      coord : {"X" : "20","Y" : "45"}
      content: {"source":"txt","value":"If you have found this card, please contact IT support Center\nBuilding 42, Brussel street\n ACME Company"}
      font: {"name": "Arial", "style": "", "size": "6"}
      style: {"width":"45","interline":"2","border":"0","alignment":"C"}
