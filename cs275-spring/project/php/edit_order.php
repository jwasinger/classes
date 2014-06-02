<?php 
session_start();
include "../php/user.php";

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

	return $items;
}

if($_SERVER['REQUEST_METHOD'] === 'GET')
{
	$_SESSION['order_id'] = $_GET['order_num'];
	include '../html/edit_order.html';
}
else
{
	$orders = get_user_orders($_SESSION['username']);
	$edit_order = null;

	for($i=0; $i < count($orders); $i++)
	{
		if($orders[$i]->order_id == $_SESSION['order_id'])
		{
			$edit_order = $orders[$i];
		}
	}

	$items = get_form_items();
	modify_order_items($edit_order->order_id, $items);
	unset($_SESSION['order_id']);

	header('Location: ../php/user_home.php');
}
?>