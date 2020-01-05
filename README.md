![badge](https://github.com/Asher0004/PPMT/workflows/C++%20CI/badge.svg)

# Personal Password Management tool
Encrypt all your data in one little file accounts.encrypted and in a very safe way using advanced cryptography.
             
```bash
             
        0000                 111111111111111      111111111111111      11111111111111111111      111111111111111
      00    00               11           11      11           11      11      1111      11            111
    00        00             11           11      11           11      11      1111      11            111
  000          000           11           11      11           11      11      1111      11            111
 00              00          111111111111111      111111111111111      11      1111      11            111
00                00         11                   11                   11      1111      11            111
00                00         11                   11                   11      1111      11            111
00                00         11                   11                   11      1111      11            111
00000000000000000000         11                   11                   11                11            111
00000000000000000000         11                   11                   11                11            111
00000000000000000000         11                   11                   11                11            111
00000000000000000000         11                   11                   11                11            111
00000000000000000000         11                   11                   11                11            111
```

## Libraries required before compilation: 

sudo apt-get install libcrypto++-dev libcrypto++-doc libcrypto++-utils --fix-missing <br>
sudo apt-get install libjsoncpp-dev --fix-missing

## Compilation:
g++ -std=c++17 authentificate.cpp -o authentificate -lcryptopp -ljsoncpp

# Usage:
```
root@root:~/Desktop/PPMT# ./authentificate
-------------------Menu---------------------
1 : Add an entry
2 : Remove an entry
3 : List Entries
4 : Manage accessing entries
5 : Quit
--------------------------------------------

1
Password for decrypt file : test
Password for data entries : datapwd
Url : google
User : potato
Password : coral
Change data ? [y/n] n
Saved entry infos succesfully.
-------------------Menu---------------------
1 : Add an entry
2 : Remove an entry
3 : List Entries
4 : Manage accessing entries
5 : Quit
--------------------------------------------

1
Password for decrypt file : test
Password for data entries : datapwd
Url : facebook
User : spidey
Password : secret
Change data ? [y/n] n
Saved entry infos succesfully.
-------------------Menu---------------------
1 : Add an entry
2 : Remove an entry
3 : List Entries
4 : Manage accessing entries
5 : Quit
--------------------------------------------

2
Password for decrypt file : test
-----------------------------------
| line |    url   | user  |  pwd  |
-----------------------------------
|    1 | google   | ***** | ***** |
|    2 | facebook | ***** | ***** |
-----------------------------------
line to supress [q:cancel]: 2
Deleted row succesfully.
-------------------Menu---------------------
1 : Add an entry
2 : Remove an entry
3 : List Entries
4 : Manage accessing entries
5 : Quit
--------------------------------------------

3
Password for decrypt file : test
---------------------------------
| line |   url  | user  |  pwd  |
---------------------------------
|    1 | google | ***** | ***** |
---------------------------------
Password for data entries : datapwd
----------------------------------
| line |   url  |  user  |  pwd  |
----------------------------------
|    1 | google | potato | coral |
----------------------------------
-------------------Menu---------------------
1 : Add an entry
2 : Remove an entry
3 : List Entries
4 : Manage accessing entries
5 : Quit
--------------------------------------------

4
----------Manage accessing entries----------
1 : Modify password decrypt/encrypt file
2 : Modify password accessing entries
3 : Main menu
4 : Quit
--------------------------------------------

1
Old password for decrypt/encrypt file : test
New password for decrypt/encrypt file : newtest
Changed password file succesfully.
-------------------Menu---------------------
1 : Add an entry
2 : Remove an entry
3 : List Entries
4 : Manage accessing entries
5 : Quit
--------------------------------------------

4
----------Manage accessing entries----------
1 : Modify password decrypt/encrypt file
2 : Modify password accessing entries
3 : Main menu
4 : Quit
--------------------------------------------

2
Password for decrypt file : newtest
Old password for data entries : datapwd
New password for data entries : newdatapwd
Changed password content succesfully.
-------------------Menu---------------------
1 : Add an entry
2 : Remove an entry
3 : List Entries
4 : Manage accessing entries
5 : Quit
--------------------------------------------

3
Password for decrypt file : newtest
---------------------------------
| line |   url  | user  |  pwd  |
---------------------------------
|    1 | google | ***** | ***** |
---------------------------------
Password for data entries : newdatapwd
----------------------------------
| line |   url  |  user  |  pwd  |
----------------------------------
|    1 | google | potato | coral |
----------------------------------
-------------------Menu---------------------
1 : Add an entry
2 : Remove an entry
3 : List Entries
4 : Manage accessing entries
5 : Quit
--------------------------------------------

5
root@root:~/Desktop/PPMT# 

```

# Example data generated:

```bash
root@root:~/Desktop/PPMT# cat accounts.encrypted
B14DE859AC535A29368519C40F88059628B5C7FE1578B46169DA02CA9AA5FA0B
150862260A2CD2E086B26B7FE194CBC18D3A75B037C5AC127290024CEE780A4E
8A0930907287AC3AF5AC017128E583048BFFA47BAE9FF9AEBD3304A17E1EF0BC
90F4348EBE267B3C159B7FC7276065BB35FAB93A41C33D84879B156A583152FA
31239BEFAA6ADEA3DD816AB6A3EFF5D438DE0296A8E1775F5D9761E952DE7167
1BB3A8C8879C02B266409FD771103905997CDAACE35B2BCCA66D3102543732A4
5CE881AD2CE03825DB18C793CDAD4F9D540DF54FE1D5C7E15CC79548A0DEC0DA
0CF32C61
root@root:~/Desktop/PPMT# 
```
