<?php 
	session_start()
	$username = mysql_real_escape_string(_POST['password']);
	$password = md5($password);
?>