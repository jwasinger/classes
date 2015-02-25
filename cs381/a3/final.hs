--CS 381 Homework #3
--Jared Wasinger, Justin Thrailkill

--1a)
type Prog = [Cmd]

data Cmd = LD Int
         | ADD 
         | MULT
         | DUP
         | INC 
         | SWAP
         | POP Int

type Rank = Int
type CmdRank = (Int, Int)

rankC :: Cmd -> CmdRank
rankC (LD i) = (0, 1)
rankC (ADD)  = (2, 1)
rankC (MULT) = (2, 1)
rankC (INC)  = (1, 1)
rankC (SWAP) = (2, 2)
rankC (POP i) = (0, i)


rank :: Prog -> Rank -> Maybe Rank
rank [] r     = Just r
rank (x:xs) r = case r + (snd (rankC x) - fst (rankC x)) >= 0 of
                      True -> rank xs (r + (snd (rankC x) - fst (rankC x)))
                      False -> Nothing
                      

rankP :: Prog -> Maybe Rank
rankP [] = Just 0 
rankP x = rank x 0

--test values....

p1 =      [LD 1, LD 2, ADD]
p2 =      [LD 1, LD 2, LD 3, SWAP, POP 1, ADD]
p_error = [LD 1, LD 2, MULT, ADD]

--1b) 

--2a)

data Shape = X
           | TD Shape Shape
           | LR Shape Shape
           deriving Show

type BBox = (Int, Int)

bbox :: Shape -> BBox
bbox (X)      = (1,1)
bbox (TD s s') = (fst (bbox s) + fst(bbox s'),  snd(bbox s) + snd(bbox s'))
bbox (LR s s') = (fst (bbox s) + fst(bbox s'),  snd(bbox s) + snd(bbox s'))

--2b)


--3) 
--[]
--[<x:?>] Push
--[<y:?, x:?>] Push
--[<y:1, x:?>]
--[<f:{}, y:1, x:?>] Push
--[<x:2, f:{}, y:1, x:?>] Push
--[<x:1>, <x:2, f:{}, y:1, x:?>] Push
--[<x:0>, <x:1>, <x:2, f:{}, y:1, x:?>] Push
--[<x:1>, <x:2, f:{}, y:1, x:?>] Pop
--[<x:2, f:{}, y:2, x:?>] Pop
--[<f:{}, y:5, x:?>] Pop
--[<y:5, x:?>] Pop
--[<x:?>] Pop
--[] Pop


--4a) 21
--4b) 39