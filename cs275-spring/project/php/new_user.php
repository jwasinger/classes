<?php 
	
	include "connect.php";

	$username = $_POST['username'];
	$password = $_POST['password'];
	$password2 = $_POST['password2'];
	//$password = md5($password); for now use plain text

	if(strcmp($password,$password2) != 0)
	{
		echo '<script> alert("Passwords are not equal!") </script>';
		header('new_user.html');
		//or exit()?
	}


	//see if the username already exists
	$query = 'SELECT * FROM Users
			  WHERE username="'.$username.'"';
	
	$result = mysql_query($query, $db_connection);
	if(!$result)
	{
		echo "mysql failed".mysql_error();
	}

	if(mysql_num_rows($result) != 0)
	{
		echo '<script language="javascript">
				 alert("account already exists") 
			  </script>';

		header('Location: ../html/new_user.html');
	}
	else
	{
		$query = 'INSERT INTO Users
				  VALUES ("'.$username.'", "'.$password.'", "customer", 0);';

		$result = mysql_query($query);

		if(!$result)
		{
			echo 'MySql error: '.mysql_error();
		}
		else
		{
			header('Location: user_home.php');
		}
	}
?>