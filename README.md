# fusiondirectory-plugins-document

This is a plugin for FusionDirectory that allow to create PDF from ldap information such :
  - identity card
  - information forms
  - what you want !


## Why this plugin ?

This plugin allow PDF generation with LDAP data such image, text.
PDF could be downloaded or directly printed to a specific printer
The first utilisation is to print acces card to a dedicted printed, it avoid to generate a poor excel file without image.
With this plugin, it is more simple, got to Document tab, choose type of output, print or download.

## Todo

## How to install

`cd /usr/local/src/
git https://github.com/gallak/fusiondirectory-plugins-document.git
mv fusiondirectory-plugins-document document`

### Installation of ldap schema

`ldap-schema-manager -i ./document/contrib/schema/document-fd.schema`

### Installation of plugin

```
 /usr/local/src/gallak/pluginsmanager/fusiondirectory-plugin-manager --set-fd_home=/usr/local/share/fusiondirectory  --plugin-name=document --plugins-archive=/usr/local/src/gallak --install-plugin
Setting fd_home to /usr/local/share/fusiondirectory
plugin document will be used
Plugins folder /usr/local/src/gallak will be used
Installing FusionDirectory's plugins
Installing plugin document
Plugin record exist : cn=document,ou=plugins,ou=fusiondirectory,dc=demo,dc=fusion
Deleting it !
Create plugin record
Scanning and update Class.cache and translations
```

This plugin need :
  * php-fpdf extension in order to generate PDF
  * php-yaml extension in order to read template system base on yaml

### Allow  printing

This plugin use php exec function.
As trigger inside FusionDirectory, it is a good pratice to create a dedicated user for this.

#### create a dedicated user for printing

    root@fd-14-dev:/usr/local/src/# adduser roneo
    Adding user `roneo' ...
    Adding new group `roneo' (1001) ...
    Adding new user `roneo' (1001) with group `roneo' ...
    The home directory `/home/roneo' already exists.  Not copying from `/etc/skel'.
    New password: 
    Retype new password: 
    passwd: password updated successfully
    Changing the user information for roneo
    Enter the new value, or press ENTER for the default
    	Full Name []: 
    	Room Number []: 
    	Work Phone []: 
    	Home Phone []: 
    	Other []: 
    Is the information correct? [Y/n] Y
    root@fd-14-dev:/usr/local/src/# 

#### Install requirement for printing   

 * install print software
    `apt-get --no-install-recommends install cups`

  * add printer and activate filequeue
```
    lpadmin -p myprinter -v ipp://192.168.0.100/printers/MFC7460DN -m everywhere
    cupsaccept myprinter
```

#### create the sudo autorisation

And this line in sudoers files

    www-data        ALL=(roneo) NOPASSWD: /usr/bin/lp

#### Create a template

All templates and associated ressources must be located in `/etc/fusiondirectory/document` folder.
Apache user could access to this folder and the folder used for PDF generator
An example of template is located in docs folder ![card template](/docs/examples/user_cardtemplate.tpl)
The small template interpretor used is store in the `class_pdfBuilder.inc` file.
It's use the fabulous fpdf plugin ( see : [FPDF](http://fpdf.org/)
All X,Y coordonates are absolute from de upper left corner.

#### Examples

##### Document configuration
![configuration of document type](/docs/document-configuration.png)
##### Document generation
![choice of docuement](/docs/document-usage.png)
##### Document examples
  * template
![card template](/docs/examples/user_cardtemplate.tpl)

  * PDF generated ( for classical card format)

![output example](/docs/examples/card-example.pdf)
