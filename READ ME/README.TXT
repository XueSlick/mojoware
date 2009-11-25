=================================================
MOJO

GUIDE TO THE SOURCE CODE FOR PROGRAMMERS

=================================================

Copyright (c) 2009 Robert Sacks.

For a complete copy of the newest version of Mojo, go to www.mojoware.org.

-------------------------------------------------
What's this file about?
-------------------------------------------------

This file provides an orientation for programmers who want to examine or change Mojo's source code.  It explains how the program is organized.


-------------------------------------------------
License
-------------------------------------------------

You may redistribute and/or modify Mojo under the terms of the GNU General Public License, version 3, as published by the Free Software Foundation.  For more information see the file "License.txt" which is part of the OSBXR package. If you did not receive that file, go to:

www.mojoware.org


------------------------------------------------
Legal
------------------------------------------------

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


-------------------------------------------------
Contact info
-------------------------------------------------

Questions, suggestions, bug reports, etc., should be posted in the forum at www.mojoware.org.


-------------------------------------------------
Compiler
-------------------------------------------------

Mojo is written for Visual Studio 2010. Some parts of the source code require TR1 features of C++.


-------------------------------------------------
Libraries
-------------------------------------------------

The program doesn't require any libraries except those that come with Visual Studio 2010 and the Windows Software Development Kit (SDK), August 2009 edition or higher.


-------------------------------------------------
Build instructions
-------------------------------------------------

Load the solution file (mojo.sln) into Visual Studio 2010 and select "Rebuild Solution."


-------------------------------------------------
Operating system
-------------------------------------------------

The program requires Windows XP service pack 1 or higher.

The program makes heavy use of the Win32 API and no attempt has been made to separate Windows-specific parts of the source code from portable sections.


-------------------------------------------------
Overall architecture
-------------------------------------------------

The program is divided into two parts.

1.  An engine.

2.  An application.

The engine does most of the work.  The application provides the user interface.  The application communicates with the engine through a narrow API which is defined in mojo_engine.h in mojo_engine.


-------------------------------------------------
Organization of the Visual Studio solution
-------------------------------------------------

The solution contains five projects:

1.  mojo_app.  This is the EXE.  It provides the user interface.  It's written in unmanaged C++.

2.  mojo_engine.  This is the main DLL.  It does most of the work.  It's written in unmanaged C++.  It exports the API used by the application.

3.  mojo_hooks.  This is a small DLL that contains the low-level keyboard hook and low-level mouse hook.  It's written in unmanaged C++.  It exports some functions to mojo_engine.  These exports should not be used by the application.

4. scrib_compiler.  This is a stand-alone console application that converts scrib files into C++ files that get linked into mojo_engine and mojo_app. For an explanation, see "International language support" below.

5.  MojoInstaller.  The application's ClickOnce installer for end users. Since ClickOnce only works with .NET programs, there is a tiny .NET program in this project.  The .NET program launches mojo.exe.


-------------------------------------------------
Namespace mojo
-------------------------------------------------

Everything exported by the engine to the application is in namespace mojo.


-------------------------------------------------
The nature of the firewall
-------------------------------------------------

The engine exports most of its own classes to the application.  At first glance it seems as if there's not much of a firewall here.

However those classes are only available to be instantiated and used by the application for its own needs.

The application has only narrow access to the engine's data (its objects). The application can only access the engine's data through a small number of non-member functions that are prototyped in mojo_engine.h.


-------------------------------------------------
Naming conventions
-------------------------------------------------

Function names are lower case.

Names of types and long-lived variables are camel case (ThisIsCamelCase).

Short lowercase names are used for variables wherever this makes the source code easier to read.

The following type-Hungarian prefixes are used:

c  = class or char
i  = int
u  = unsigned int
dw = DWORD
p  = pointer
a  = array
w  = word or wchar_t
aw = array of wchar_t
d  = double
f  = float
y  = BYTE
t  = template
h  = handle
s  = string

Class objects usually begin with a capital letter, without a prefix.


-------------------------------------------------
Window wrapper classes
-------------------------------------------------

The application uses a set of classes to wrap windows.  The primary base class for these wrappers is cWin.  The wrappers serve two main purposes.

1.  They resize child windows (particularly controls in dialog boxes) automatically when the parent window changes size.  This functionality is provided by the eAnchor stuff in cWin.

2.  They transfer data between the program and dialog box controls. 

For each window in the program, a class is derived from cWin.  Each class contains its own window proc.   When the window is created, the window proc stores the address of the cWin object in the window's user data.  Each time the window proc is called, it extracts this address and uses it to call member functions of cWin.


-------------------------------------------------
Helper classes for cWin
-------------------------------------------------

Class cWin is helped by:

1.  cDlgVars, which transfers data between dialog controls and class cSettings.

2.  The cScrib system which is responsible for multi-language text.  See "International language support" below.


-------------------------------------------------
Settings
-------------------------------------------------

Both the engine and application use class cSettings to store settings on disk and hold them in memory.  Each of the two components has its own settings and its own settings file.  The application can change the engine's settings via a function called mojo::set().


-------------------------------------------------
International language support
-------------------------------------------------

Both the engine and application use "scribs" to display UNICODE text in multiple languages.

A scrib is a string of text with embedded printf format fields.   At the moment when the program displays a scrib, it replaces those fields with dynamic data.

Scribs are supplied to the program as either (1) files loaded at run time or (2) linked, compiled C++ code.  A file can be either 16-bit Unicode or 8-bit ANSI.  The program detects the type automatically.

The compiled program contains linked, compiled scribs in English for all of the program's text.   This ensures that English default text is always available to the program even if an alternative scrib file fails to load properly or contains errors.

The scrib system includes five classes:

1. cScrib represents a string of text in the form of a printf-style format string.

2. cScribPack contains two scribs (a headline and a body).  

3. cScribMgr loads scribs from files and/or C++ source into a C++ STL multimap.

4. cMemo derives from cScribPack.  It represents a complete message (including severity level) that can be displayed to the user in a message box or monitor window.

5.  cException derives from cMemo.  This is the class that gets thrown as a C++ exception.


-------------------------------------------------
Details about scribs
-------------------------------------------------

Scribs are loaded from files or C++ data structures.  Each line of input has the following form:

key = value

Where both key and value are text.  The ScribMgr reads these files and loads them into a C++ STL multimap.

A value uses the same syntax as a printf format string.  This allows the program to insert fields of various types into a line of text before displaying it.

This sounds fragile and risky but the code that performs the substitution is wrapped in SEH try-catch blocks so it's actually bullet proof.

The substitution code is in replace_format_specification_fields() in utility_private.cpp.

When the program needs to display text, it (1) uses a key to (2) locate appropriate values in the cScribMgr's multimap, which (3) are loaded into cScribs where (4) the format specification fields get replaced by variables.  

Class cWin contains a member function, set_item_text, which handles those actions for window text.


-------------------------------------------------
Instances
-------------------------------------------------

Only one instance of the engine (loaded by a single application) should exist on a computer at one time.  The application tries to ensure this with code that's contained in single_instance.cpp in mojo_app.  (The engine should probably try to enforce this too.)


-------------------------------------------------
Relationship between engine's threads and EXE
-------------------------------------------------

The engine does its work in threads that are completely separate from any threads that may be created by the application.

The application doesn't need to know anything about the engine's threads. 

The engine communicates with the application by sending Windows messages to the application's main window.  These messages are defined in mojo_engine.h.   The messages tell the application to retrieve various pieces of data from circular buffers.  These buffers are thread safe because they use critical sections to protect themselves from access by multiple threads.  To avoid possible race conditions and delays it's recommended that applications use some single thread to retrieve data from these buffers.  In most cases the best choice will probably be the thread that runs the message loop of the application's main window.


-------------------------------------------------
The engine's threads
-------------------------------------------------

The engine has five main threads.  One thread (with entry point in mojo_hooks.cpp of mojo_engine) installs and services the low-level hooks.  The second thread ("messenger thread") figures out whether a hotkey has been pressed and queues commands to be sent to other computers.  The third thread ("interpreter thread") executes the script language.  The fourth thread broadcasts UDP packets to try to find other instances of the program on the network.  The fifth thread listens for those broadcasts.  The fourth and fifth threads run intermittently at low priority so their impact on performance is negligible.

All these threads are in wait states most of the time, and when they run they usually relinquish their quantums almost immediately.

In addition, there is a thread pool associated with the IoCompletionPort that services TCP socket communications.  The number of threads in this pool is controlled dynamically by Microsoft's thread pool code.


-------------------------------------------------
Hooks
-------------------------------------------------

The engine installs a low-level keyboard hook and a low-level mouse hook.  These hooks are in a small DLL (project mojo_hooks) by themselves. They get installed by their own thread.   They get called by the  operating system in the context of that thread.   All they do is call exported functions in the big DLL (project mojo_engine).


-------------------------------------------------
UDP network discovery
-------------------------------------------------

The program uses UDP broadcast to find copies of itself on the local network.   This  work is done by class cFinder.  The application's user interface calls this feature "Auto Find."  Class cFinder uses two dedicated threads for this work, one for sending and one for listening.   The use of two dedicated threads sounds inefficient but they run infrequently and at low priority so their impact on performance is neglible.


-------------------------------------------------
TCP Communications
-------------------------------------------------

The program uses Winsock2 and an IoCompletionPort to communicate with copies of itself on other computers via TCP. 

For each pair of computers, two connections are maintained.   In one connection, computer A is the server and B is the client; in the other connection, the roles are reversed.  Thus every computer is both a server and client with respect to every other computer.

The reason why there are two connections is because Winsock sends more rapidly from client to server than it does from server to client.  More rapidly in the sense of a machine gun, i.e., more individual transmissions per second.  This is extremely important for KM (mouseover).  Since all instances of Mojo are equal (all can send commands), all instances need client-to-server connections to all other instances.

The TCP communications code is in class cPool.  (The name refers to the  pool of worker threads that services the IoCompletionPort.)