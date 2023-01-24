<?php
/*
  This code is part of FusionDirectory (http://www.fusiondirectory.org/)

  Copyright (C) 2013-2020 FusionDirectory

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

class documentConfig extends simplePlugin
{


  static function plInfo (): array
  {
    return [
      'plShortName'     => _('Document'),
      'plTitle'         => _('Document configuration'),
      'plDescription'   => _('FusionDirectory Document plugin configuration'),
      'plObjectClass'   => ['fdDocumentPluginConf'],
      'plPriority'      => 10,
      'plObjectType'    => ['configuration'],

      'plProvidedAcls'  => parent::generatePlProvidedAcls(static::getAttributesInfo())
    ];
  }


  static function getAllTemplates(): array
  {
    $docdir_tpl = CONFIG_DIR."document";
    $all_templates=array();
    if (scandir($docdir_tpl)){
      $tpl_files=scandir($docdir_tpl);

      foreach ($tpl_files as $file){
        $val=preg_match('/(.*)\.tpl$/',$file,$name);
        if (! empty($val)) {
          $all_templates[]=$name[0];
        }
      }
      return($all_templates);
    }else{
    $error = new FusionDirectoryError(
      htmlescape(sprintf(
        _('Cannot read folder: "%s"'),
        $docdir_tpl
      ))
    );
    $error->display();
    return(array());
    }
    
  }




  static function getAttributesInfo (): array
  {
    global $config;

    $plugins = [];
    if (session::is_set('plist')) {
      $plugins = array_keys(session::get('plist')->info);
    }
    sort($plugins);

    $alltemplates = documentConfig::getAllTemplates();

    return [
      'main' => [
        'name'  => _('Document'),
        'class' => ['fullwidth'],
        'attrs' => [
          new StringAttribute(
            _('Print Command'), _('Print Command, first argument is the printer , second one the document'),
            'fdDocumentPrintCommand', TRUE ,'lp -d %s %s'
          ),
          new StringAttribute(
            _('Print Folder'), _('Print Folder'),
            'fdDocumentPrintFolder', TRUE ,'/var/cache/fusiondirectory/tmp'
          ),
          new OrderedArrayAttribute(
            new CharSeparatedCompositeAttribute(
              _('Document list'),
              'fdDocumentList',
              [
                new SelectAttribute(
                  _('ObjectType'), _('Type of object related to'),
                  'fdDocumentList_objectType', TRUE,$plugins
                ),
                new SelectAttribute(
                  _('Document'), _('Label of Document'),
                  'fdDocumentList_DocumentLabel', TRUE,$alltemplates
                ),
                new stringAttribute(
                  _('Description'), _('Description of Document'),
                  'fdDocumentList_DocumentDesc', TRUE
                ),                
                new SelectAttribute(
                  _('Action'), _('Policy of document'),
                  'fdDocumentList_Action', TRUE,
                  array('download','print'), '',array('download','print')
                ),
                new stringAttribute(
                  _('printer/filename'), _('Default printer for document or filename'),
                  'fdDocumentList_printer', FALSE
                ),
              ],
              ';',
              '',
              _('Document')
            ),
            FALSE,
            [],
            TRUE
            ),
         ],
      ]
    ];
  }

  function __construct ($dn = NULL, $object = NULL, $parent = NULL, $mainTab = FALSE)
  {
    global $config;

    parent::__construct($dn, $object, $parent, $mainTab);

 }

}
