docker run --rm -i -t -p 8080:8080 -p 443:443 -v /tmp:/roboscans/tmp -v /var/www/logging:/roboscans/logging -v /var/www/manga:/roboscans/manga -v /var/www/main:/roboscans/www registry.gitlab.com/giveandtake/diogenes:latest -v /var/www/certs:/roboscans/certs

