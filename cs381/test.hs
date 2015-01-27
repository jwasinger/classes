--data Sentence = Phrase Noun Verb Noun
--              | And Sentence Sentence
                --deriving Show

--instance Show Sentence where 
--    show (Phrase s v o) = show s++" "++show v++" "++show o
--    show (And s1 s2)    = show s1++" and "++show s2
    
--data Noun = Dogs | Teeth deriving Show



--data Verb = Have deriving Show

--data Digit = One | Zero deriving Show
--data Bin = D Digit | B Digit Bin deriving Show

--l0l = B One (B Zero (D One))

--data Boolean = T | F | Not Boolean

data PeanoNum = Zero | Succ PeanoNum
one = Succ Zero
two = Succ one
three = Succ two
