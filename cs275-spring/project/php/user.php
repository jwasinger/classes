<?php 
require_once('connect.php');

function get_user_wallet($username)
{
	$query = mysql_query('SELECT wallet_amt 
						  FROM Users
						  WHERE username="'.$username.'"');
	if(!$query)
	{
		echo "MySQL error!";
		return null;
	}
	else
	{
		return $query;
	}
}

class Order
{
	public $username;
	public $order_id;
	public $price;
	public $items;
	public $combos;
}

class Combo
{
	public $name;
	public $items;
}

class Item
{
	public $name;
	public $price;

	public function __construct($name, $price)
	{
		$this->name = $name;
		$this->price = $price;
	}
}

function get_all_items()
{
	$items = array();

	$query = mysql_query('SELECT * FROM Items');
	if(!$query)
	{
		echo "MySQL error: ".mysql_error();
		die();
	}

	while($row = mysql_fetch_array($query))
	{
		$items[] = new Item($row[0], $row[1]);
	}

	return $items;
}

function get_highest_order_id()
{
	$query = mysql_query('SELECT MAX(order_id) FROM Orders');
	if(!$query)
	{
		echo "error: ".mysql_error();
		die();
	}

	return mysql_fetch_array($query)[0];
}

function calculate_items_cost($items)
{
	$count = 0;
	for($i = 0; $i < count($items); $i++)
	{
		$count += $items[$i]->price;
	}

	return $count;
}

function delete_user_order($order_id)
{
	$query = mysql_query('DELETE FROM Orders 
						  WHERE order_id ='.$order_id);

	if(!$query)
	{
		echo "MySQL Error: ".mysql_error();
		die();
	}
}

function get_user_orders($username)
{
	$orders_query = mysql_query('SELECT * FROM Orders
						   		 WHERE customer="'.$username.'"');

	$orders = array();

	if(!$orders_query)
	{
		echo "Error: ".mysql_error();
		die();
	}

	while($row = mysql_fetch_array($orders_query, MYSQL_NUM))
	{
		$order = new Order();
		$order->username = $row[2];
		$order->order_id = $row[1];
		$order->price = $row[0];

		$order->items = array();

		$items_query = mysql_query('SELECT order_id, item_name, price
									FROM Items_Orders
									INNER JOIN Items ON Items_Orders.item_name = Items.name
									WHERE order_id = "'.$order->order_id.'"
									');
		if(!$items_query)
		{
			echo 'MySQL error: '.mysql_error();
			die();
		}

		//add each item to the order
		while($items_row = mysql_fetch_array($items_query))
		{
			$item = new Item($items_row[1], $items_row[2]);
			$order->items[] = $item;
		}

		$orders[] = $order;
	}

	return $orders;
}

function get_order_items_counts($order_id, $username)
{
	$dict = null;
	$target_order = null;

	$all_orders = get_user_orders($username);
	for($i=0; $i<count($all_orders); $i++)
	{
		if($all_orders[$i]->order_id == $order_id)
		{
			$target_order = $all_orders[$i];			
		}
	}

	for($i=0; $i<count($target_order->items); $i++)
	{
		if(isset($dict[$target_order->items[$i]->name]))
		{
			$dict[$target_order->items[$i]->name] += 1;
		}
		else
		{
			$dict[$target_order->items[$i]->name] = 1;
		}
	}

	return $dict;
}

// function __update_orders_items_relation($order_id, $new_items, $old_items_counts)
// {
// 	$small_fries_offset = 0;
// 	$small_drink_offset = 0;

// 	$new_items_counts = array();

// 	for($i=0; $i<count($new_items); $i++)
// 	{
// 		if(isset($new_items_counts[$new_items[$i]->name]))
// 		{
// 			$new_items_counts[$new_items[$i]->name] += 1;
// 		}
// 		else
// 		{
// 			$new_items_counts[$new_items[$i]->name] = 1;
// 		}
// 	}

// 	//calculate the offsets
// 	if(isset($new_items_counts['small_fries']) && isset($old_items_counts['small_fries']))
// 	{
// 		$small_fries_offset = $new_items_counts['small_fries'] - $old_items_counts['small_fries'];
// 	}
// 	else if(isset($new_items_counts['small_fries']) && !isset($old_items_counts['small_fries']))
// 	{
// 		$small_fries_offset = $new_items_counts['small_fries'];
// 	}

// 	if(isset($new_items_counts['small_drink']) && isset($old_items_counts['small_drink']))
// 	{
// 		$small_drink_offset = $new_items_counts['small_drink'] - $old_items_counts['small_drink'];
// 		echo "hey";
// 	}
// 	else if(isset($new_items_counts['small_drink']) && !isset($old_items_counts['small_drink']))
// 	{
// 		$small_drink_offset = $new_items_counts['small_drink'];
// 		echo "sup";
// 	}

// 	echo $old_items_counts['small_fries'].' '.$old_items_counts['small_drink'];
// 	echo '<br>';
// 	echo count($new_items).' '.$small_fries_offset.' '.$small_drink_offset;
	
// 	die();

// 	//update tables
// 	if($small_fries_offset < 0)
// 	{
// 		$small_fries_offset *= -1;
// 		for($i=0; $i<$small_fries_offset; $i++)
// 		{
// 			$query = mysql_query('DELETE FROM Items_Orders
// 								  WHERE order_id='.$order_id.' AND item_name=\'small_fries\'
// 								  LIMIT 1');

// 			if(!$query)
// 			{
// 				echo "MySQL Error: ".mysql_error();
// 				die();
// 			}
// 		}
// 	}
// 	else if($small_fries_offset > 0)
// 	{
// 		for($i=0; $i<$small_fries_offset; $i++)
// 		{
// 			$query = mysql_query('INSERT INTO Items_Orders (order_id, item_name)
// 								  VALUES ('.$order_id.', item_name="small_fries")
// 								  ');

// 			if(!$query)
// 			{
// 				echo "MySQL Error: ".mysql_error();
// 				die();
// 			}
// 		}
// 	}

// 	if($small_drink_offset < 0)
// 	{
// 		$small_drink_offset *= -1;
// 		for($i=0; $i<$small_drink_offset; $i++)
// 		{
// 			$query = mysql_query('DELETE * FROM Items_Orders
// 								  WHERE order_id='.$order_id.' AND item_name=\'small_drink\'
// 								  LIMIT 1');

// 			if(!$query)
// 			{
// 				echo "MySQL Error: ".mysql_error();
// 				die();
// 			}
// 		}
// 	}
// 	else if($small_drink_offset > 0)
// 	{
// 		for($i=0; $i<$small_drink_offset; $i++)
// 		{
// 			$query = mysql_query('INSERT INTO Items_Orders (order_id, item_name)
// 								  VALUES ('.$order_id.', item_name="small_drink")
// 								  ');
			
// 			if(!$query)
// 			{
// 				echo "MySQL Error: ".mysql_error();
// 				die();
// 			}
// 		}
// 	}
// }

function modify_order_items($order_id, $items)
{
	$query = mysql_query('SELECT * FROM Orders 
						  WHERE order_id='.$order_id.'');

	if(!$query)
	{
		echo "MySQL Error: ".mysql_error();
		die();
	}

	$new_order_cost = calculate_items_cost($items);
	$query = mysql_query('UPDATE Orders 
						  SET cost='.$new_order_cost.'
						  WHERE order_id='.$order_id);

	if(!$query)
	{
		echo "MySQL Error: ".mysql_error();
		die();
	}

	//update Items_Orders relation
	$query = mysql_query('DELETE FROM Items_Orders
						  WHERE order_id='.$order_id);
	if(!$query)
	{
		echo "MySQL Error: ".mysql_error();
		die();
	}

	for($i = 0; $i < count($items); $i++)
	{

		$query = mysql_query('INSERT INTO Items_Orders (order_id, item_name)
							  VALUES ('.$order_id.', "'.$items[$i]->name.'")');

		if(!$query)
		{
			echo "MySQL error:".mysql_error();
			die();
		}
	}
}

function add_user_order($username, $items)
{
	$new_order_id = get_highest_order_id() + 1;
	$new_order_cost = calculate_items_cost($items);
	$query = mysql_query('INSERT INTO Orders (cost, order_id, customer)
						  VALUES ('.$new_order_cost.','.$new_order_id.', "'.$username.'")');

	if(!$query)
	{
		echo "MySQL error: ".mysql_error();
		die();
	}

	for($i = 0; $i < count($items); $i++)
	{

		$query = mysql_query('INSERT INTO Items_Orders (order_id, item_name)
							  VALUES ('.$new_order_id.', "'.$items[$i]->name.'")');

		if(!$query)
		{
			echo "MySQL error:".mysql_error();
			die();
		}
	}
}


?>