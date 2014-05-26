<?php 
	$user_login_fail = false;

	session_start()
	$username = mysql_real_escape_string(_POST['password']);
	$password = md5($password);

	//see if the user entered the correct credentials
	$login = mysql_query("SELECT * FROM users 								
						  WHERE (username='".$username."') AND (password='".$password"')");

	if(mysql_num_rows($login) == 1)
	{
		$_SESSION['username'] = $_POST['username'];
		header('Location: user_home.php');
	}
	else
	{
		$user_login_fail = true;
		header('Location: index.php');
	}

?>