# libdsm
<p>&nbsp&nbsp This is a rewrote library based on the original libdsm library of Videolabs's organization.<br>
&nbsp&nbsp The original libdsm you can visit here: https://github.com/videolabs/libdsm.<br>
&nbsp&nbsp This library was developed to support iOS developers who want to handly implement this library to their projects instead of using static library that can not rewrite for personal purposes</p>
<h2>About</h2>
<p>&nbsp&nbsp lib Defective SMb (libDSM) is a SMB protocol client implementation in pure old C, with a lot less features than Samba but is simpler, and has with a more permissive license (currently LGPL + proprietary).<br>
&nbsp&nbsp The initial goal of this project is to have a library that can access most SMB shares to read files and that has with a license compatible with the iOS/Android/WinRT appstores in order to integrate it into VLC for iOS and VLC for Android.</p>
  
<h2>Features</h2>
<p>
Here's a list of the currently supported features:
<ul>
  <li>NETBIOS</li>
  <ul>
    <li>Basic bi-directionnal NETBIOS name resolution</li>
    <li>Hacky LAN SMB servers discovery (Listing all the smb servers on the LAN, no WINS, etc.)</li>
    <li>Basic NETBIOS Session transport layer</li>
  </ul>
  <li>SMB</li>
  <ul>
    <li>Support only required parts of 'NT LM 0.12' (aka CIFS?) dialect.</li>
    <li>User based authentication</li>
    <li>List Shares</li>
    <li>Browse folders</li>
    <li>Read file</li>
    <li>Write file</li>
    <li>Delete file</li>
    <li>Delete empty folder</li>
    <li>Move and rename file/folder</li>
    <li>Create new folder</li>
    <li>No copy, lock, RPCs, etc. [Hum... yet]</li>
  </ul>
</ul>
</p>
<h2>Requirements</h2>
<p>&nbsp&nbsp libdsm works on iOS 7.0+ and is compatible with ARC projects.</p>
