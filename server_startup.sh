#!/bin/bash
echo "Starting nginx\n"
nginx -c /etc/nginx/nginx.conf
echo "Starting Server\n"
/usr/local/bin/server

