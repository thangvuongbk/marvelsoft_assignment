# marvelsoft_assignment

1.	Basic development and runtime environment specifications (compiler version, IDE used,
target OS)
IDE: Codeblock version 20.03 run in Ubuntu 16.04 LTS
Compiler: GNC GCC c++11
Target OS: Linux, could be used for Windows as well

2.	Class and sequence diagrams (UML)
2.1: Conceptual process
![image](https://user-images.githubusercontent.com/5207168/178246999-7ca30da8-e154-4193-bf78-f465dc5ff40b.png)
Process: 1 process
Thread:
Main: This thread is used to spawn other threads and keeps them alive
InputHandler: This thread is used to handle the json input
	           - Parse out each element 
           - push common data structure as a package order request to ProcessHandler
ProcesHandler: Receive input from InputHandler thread and process it
		- Every symbol shall have a own thread 
OutputHandler: Receive input from ProcesHandler and write to file or print it out to console

2.2	Class diagram
![image](https://user-images.githubusercontent.com/5207168/178247079-f9d72509-129c-4bec-9858-d448fa1cf981.png)

3.	The source code
Pls access my github by following link:
https://github.com/thangvuongbk/marvelsoft_assignment.git

4.	The unit tests code
Sorry but no official unit test like google UT is performed. Instead, I used the and change with the test cased based on the input json file

5.	The build system files
This is included in above github

6.	Some images for result
![image](https://user-images.githubusercontent.com/5207168/178247159-42a956ca-6d0f-4a96-9d2c-e5ba3f4f7caa.png)

Note 1:
1.1: default input json file is put under “in” folder and “out” folder is output generated
 

2. 3rd lib party used
- jsoncpp: https://github.com/open-source-parsers/jsoncpp
Could be installed via command: sudo apt install libjsoncpp-dev
