superadmin :
	gcc superadmin.c -o superadmin
new_admin : superadmin
	./superadmin
server : 
	gcc server.c -o server -l pthread
client : server
	gcc client.c -o client -l pthread