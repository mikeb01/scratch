import Text.XML.Expat.SAX
import qualified Data.ByteString.Lazy as L
import qualified Data.ByteString.Lazy.Char8 as LC

main = do
    let doc = LC.pack("<a><b>c</b><d>e</e></a>")
    parse def