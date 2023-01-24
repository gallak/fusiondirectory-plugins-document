# fusiondirectory-plugins-document

This is a plugin for FusionDirectory that allow to create PDF froem ldap information such :
  - identity card.
  - information forms


## Why this plugin ?

This plugin allow PDF generation with LDAP data suche image, text.
The first utilisation is to print acces card to a dedicted printed, it avoid to generate a poor excle file without image.
with this plugin, it is more simple, got to Document tab, choose type of output, print or download.

## Todo

## How to install

`git https://github.com/gallak/fusiondirectory-plugins-document.git
mv fusiondirectory-plugins-document document`

### Installation of ldap schema

`ldap-schema-manager -i ./document/contrib/schema/document-fd.schema`

### Installation of plugin

```fusiondirectory-setup --set-fd_home=/usr/local/share/fusiondirectory --install-plugins
Setting fd_home to /usr/share/fusiondirectory
Installing FusionDirectory's plugins
Where is your plugins archive or folder?:
<complete current path>            
Available plugins:
1:document

fusiondirectory-setup --update-cache --update-locales
```

This plugin need :
php-fpdf extension in order to generate PDF
php-yaml extension in order to read template system base on yaml

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

    apt-get --no-install-recommends install cups

  * add printer and activate filequeue

    lpadmin -p myprinter -v ipp://192.168.0.100/printers/MFC7460DN -m everywhere
    cupsaccept myprinter


#### create the sudo autorisation

And this line in sudoers files

    www-data        ALL=(roneo) NOPASSWD: /usr/bin/lp


### usage


