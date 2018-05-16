#!/bin/bash
echo Running Server Setup;
mkdir -p /roboscans/tmp/
cd /roboscans/tmp/ && mkdir -p 1 2 3 4 5 6 7 8 9 0 && \
chmod -R 777 /roboscans/tmp/{1,2,3,4,5,6,7,8,9,0};
mkdir /roboscans/manga && chmod -R 777 /roboscans/manga; 
mkdir /roboscans/www && chmod -R 777 /roboscans/www;
mkdir /roboscans/logging && chmod -R 777 /roboscans/logging;
mkdir /roboscans/certs && chmod -R 600 /roboscans/certs;
