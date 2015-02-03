my_length :: [a] -> Int
my_length (x:xs) = my_length xs + 1
my_length [] = 0

revmap :: (a->b)->[a]->[b]
revmap f (x:xs) = revmap f xs ++ [f x]
revmap f [] = []
