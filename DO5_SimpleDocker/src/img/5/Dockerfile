FROM ubuntu 
WORKDIR /home/
COPY ./nginx.conf /etc/nginx/nginx.conf
COPY ./server.c .
COPY ./part4.sh .
RUN apt-get update && apt-get install -y gcc spawn-fcgi libfcgi-dev && rm -rf /var/lib/apt/lists
RUN useradd edr1
USER edr1
ENTRYPOINT  [ "sh", "./part4.sh" ]