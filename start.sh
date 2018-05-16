docker run --rm -p 80:80 -p 443:443 -i -d -t -v /home/roboscans/tmp:/roboscans/tmp -v /etc/letsencrypt/live/dummydomain:/roboscans/certs -v /home/roboscans/logging:/roboscans/logging -v /home/roboscans/manga:/roboscans/manga -v /home/roboscans/www:/roboscans/www registry.gitlab.com/giveandtake/diogenes:latest

goaccess -f /home/roboscans/logging/access.log --real-time-html --ws-url=somethingsomething.dummydomain.com --ignore-crawlers -o /var/www/goaccess/rp.html --daemonize

