openssl genrsa -out ca.key 4096

openssl req -new -x509 -nodes -days 2048 -key ca.key -out ca.crt -subj "/C=FR/ST=IlE-DE-FRANCE/L=Le Kremlin-Bicetre/O=EPITA/OU=NET2/CN=NET2"

openssl genrsa -out net2.example.org.key 2048

openssl req -new -key net2.example.org.key -out net2.example.org.csr -subj "/C=FR/ST=IlE-DE-FRANCE/L=Le Kremlin-Bicetre/O=EPITA/OU=NET2/CN=net2.example.com"

touch net2.example.org.ext

openssl x509 -req -in net2.example.org.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out net2.example.org.crt -days 2048 -sha256 -extfile net2.example.org.ext