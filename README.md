# libdsm
<p>&ensp; This is a rewrote library based on the original libdsm library of Videolabs's organization.<br>
&ensp; The original libdsm you can visit here: https://github.com/videolabs/libdsm.<br>
&ensp; This library was developed to support iOS developers who want to handly implement this library to their projects instead of using static library that can not rewrite for personal purposes</p>
<h2>About</h2>
<p>&ensp;lib Defective SMb (libDSM) is a SMB protocol client implementation in pure old C, with a lot less features than Samba but is simpler, and has with a more permissive license (currently LGPL + proprietary).<br>
&ensp;The initial goal of this project is to have a library that can access most SMB shares to read files and that has with a license compatible with the iOS/Android/WinRT appstores in order to integrate it into VLC for iOS and VLC for Android.</p>
<h2>Features</h2>
<p>&ensp;Here's a list of the currently supported features:<br>
&ensp;&ensp;&ensp;- NETBIOS<br>
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;+ Basic bi-directionnal NETBIOS name resolution<br>
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;+ Hacky LAN SMB servers discovery (Listing all the smb servers on the LAN, no WINS, etc.)<br>
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;+ Basic NETBIOS Session transport layer<br>
&ensp;&ensp;&ensp;- SMB<br>
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;+ Support only required parts of 'NT LM 0.12' (aka CIFS?) dialect.<br>
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;+ User based authentication<br>
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;+ List Shares<br>
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;+ Browse folders<br>
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;+ Read file<br>
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;+ Write file<br>
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;+ Delete file<br>
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;+ Delete empty folder<br>
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;+ Move and rename file/folder<br>
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;+ Create new folder<br>
&ensp;&ensp;&ensp;&ensp;&ensp;&ensp;+ No copy, lock, RPCs, etc. [Hum... yet]<br>
</p>
<h2>Requirements</h2>
<p>&ensp;libdsm works on iOS 7.0+ and is compatible with ARC projects.</p>
