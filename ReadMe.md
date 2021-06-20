Run the superadmin program : gcc superadmin.c - To add new admins
Existing admin
userId : admin1234
password : password
All other data files are empty

Running client program : gcc client.c
Running server program : gcc server.c -pthread
Remove the socket file if it persists on every run of the server. If you restart the system the socket file should naturally disappear as proper closing is being ensured. 

run ./refresh.sh to clear all data files except the admin file
