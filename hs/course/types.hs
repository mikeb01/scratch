newtype Price = Price Int
    deriving (Eq, Show, Ord)
priceToInt (Price i) = i

newtype Quantity = Quantity Int
    deriving (Eq, Show, Ord)
quantityToInt (Quantity i) = i

newtype InstructionId = InstructionId Int
    deriving (Eq, Show, Ord)
instructionIdToInt (InstructionId i) = i


data Order = Order InstructionId Price Quantity
    deriving (Eq, Show, Ord)

getInstructionId (Order instructionId _ _) = instructionId

-- instance Eq Order where

--instance Show Order where
--    show (Order instructionId price quantity) =
--        "Order(" ++ show instructionId ++ "," ++ show price ++ "," ++ show quantity ++ ")"