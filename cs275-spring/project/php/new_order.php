<?php 

// function validate_order()
// {
// 	$items = get_all_items();
// 	for($i=0; $i<count($items); $i++)
// 	{
// 		if(!$_POST[$items[$i]->name.'_qty'] == 1)
// 		{
// 			return false;
// 		}

// 		if($_POST[$items[$i]->name.'_qty'] < 0)
// 		{
// 			return false;
// 		}
// 	}

// 	return true;
// }

//get the items the user ordered based on values contained in the submitted POST form
function get_form_items()
{
	$items = array();

	for($i = 0; $i < $_POST['small_fries_qty']; $i++)
	{
		$items[] = new Item('small_fries', 1.5);
	}

	for($i = 0; $i < $_POST['small_drink_qty']; $i++)
	{
		$items[] = new Item('small_drink', 2.5);
	}

	for($i = 0; $i < $_POST['cheese_burger_qty']; $i++)
	{
		$items[] = new Item('cheese_burger', 4.2);
	}

	for($i = 0; $i < $_POST['veggie_burger_qty']; $i++)
	{
		$items[] = new Item('veggie_burger', 2);
	}
	
	for($i = 0; $i < $_POST['ice_cream_qty']; $i++)
	{
		$items[] = new Item('ice_cream', 1);
	}

	return $items;
}


require_once('connect.php');
include 'user.php';

@session_start();

if($_SERVER['REQUEST_METHOD'] === 'GET')
{
	include '../html/new_order.html';
}
else
{
	$username = $_SESSION['username']; //TODO: allow other usernames to be used!
	$items = get_form_items();
	add_user_order($username, $items);

	header('Location: ../php/user_home.php');
}

?>