# SocketProgramming
<h2>CS 252 Project</h2>

In this project you will implement a P2P network for searching and downloading files. The overview is as follows,</br>

<ul>
<li>
There is a network of clients which are interconnected with each other based on a specified topology.</br>


<li>
A client (which you will code) will be provided with the following information as arguments. Note the same client code will be run multiple times with different arguments to mimic different clients.</br>
<ul>
  <li>
A directory path: this is specific to this client. The client is the owner of all files present in that directory. Another client may be searching for these files to download.</br>
</li>
<li>
A path to a configuration file, which will include the following. See sample file contents below.
  </li>
  
<ul>
  <li>
Client ID and port on which it is ready to listen for incoming connections
    <\li>
      <li>
A list of immediate neighbors and the ports they are listening on. The client is supposed to have direct connections with these neighbors for search purposes.
      </li>
      <li>
A list of files which the client should search for in the network. These files the client does not possess originally, hence it wants to search and finally download them. The downloaded files should be stored in the above mentioned directory path under a folder “Downloaded”. These files should NOT be made available to others who are searching ,since this client is not the owner.
      </li>
  </ul>
</ul>
</li>
<li>
Each of the n clients upon initialization should process its arguments; setup connections with its immediate neighbors and print relevant output (see details below).
</li>
<li>
After this, each client should search for certain files as specified in the configuration file. It can search only upto a specified depth d. For example d==2 means, it will search clients upto 2 hops away from it.
</li>
<li>
If a file is found at another client, it should setup a separate tcp connection (if not already present) with that client and receive the file. After reception, this connection must be closed. Note File search should happen only via the initial connections specified. 
</li>
</ul>




<h3>Phase 1</h3>
In this phase, the client should just process the input arguments, establish connections with its immediate neighbors and output the following.

<h3>Phase 2</h3>

In this phase, you should implement file searching to a depth of just 1, i.e. check if file is present among immediate neighbors. Each client, for each file it is supposed to search for, it must ask its immediate neighbors and find out if the neighbor is the owner. In case there are multiple owners, choose the one with the smallest unique ID. Now for each file (in ascending order of file name), the client must print “Found <filename> at <client-unique-ID> with MD5 <hash> at depth <depth>”. In case no owner is found, print 0 in <client-unique-ID> and <depth>. For this phase, just set <hash> to be 0. The depth here is 1.


<h3>Phase 3</h3>

Very similar to Phase2, except that in this phase, we will actually transfer the file if the file is found, else nothing happens. The file transfer will happen over the connection that has already been set up in phase 1. The file which is received should show up in the “Downloaded” directory of the receiver. You must then calculate the MD5 hash of the received  file and report it in the <hash> field defined in phase 2.


<h3>Phase 4</h3>

This phase is similar to Phase2 except  we will increase the search depth to 2. How to implement depth of 2 is up to you. No need to transfer the file yet. The depth should be reported accordingly.  You should go to depth 2 only if the file is not found at depth 1. For a given depth, the tie-breaking rule is the same as phase 2. Note this phase does not involve file transfer, only search. Hence no need for MD5 sum value.

<h3>Phase 5</h3>

In this phase, we will transfer the file at depth 2 also much like Phase3. Again details of the implementation is up to you, except that if the file is not at your immediate neighbor, you should use a separate connection to get the file from the node.  If a new connection is created for transfer, it must be closed after the transfer is complete and must not be used for searching files in future. All searching must strictly happen on the initial connections between immediate neighbors only.  Much like before all downloaded files should be listed in the “Downloaded” folder.
