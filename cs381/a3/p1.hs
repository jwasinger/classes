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
