<?php 
	session_start()
	$username = mysql_real_escape_string(_POST['password']);
	$password = md5($password);

	//see if the user entered the correct credentials
	$login = mysql_query("SELECT * FROM users 								\
						  WHERE (username='".$username."') AND (password=)
		")
?>