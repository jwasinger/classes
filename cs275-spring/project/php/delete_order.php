<?php 
session_start();
include "user.php";

delete_user_order($_POST['order_num']);

header('Location: ../php/user_home.php');
?>
