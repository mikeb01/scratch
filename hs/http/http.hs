
import Network.HTTP.Types
import Network.HTTP.Headers
import Network.HTTP.Conduit
import Control.Applicative
import Network (withSocketsDo)
import qualified Data.ByteString.Lazy.Char8 as L
import qualified Data.ByteString.Char8 as BC


-- Fetch a URL via GET

--fetch :: String -> IO String
--fetch url = do
--  response <- simpleHttp url
--  body <- getResponseBody response
--  return body

---- Fetch n chars from a url

--fetch_chunk :: String -> Int -> IO [Char]
--fetch_chunk url n =
--  simpleHTTP (getRequest url) >>=
--    fmap (take n) . getResponseBody

format_login :: String -> String -> String -> String
format_login user pass domain =
    "<req><body>" ++
    "<username>" ++ user ++ "</username>" ++
    "<password>" ++ pass ++ "</password>" ++
    "<productType>" ++ domain ++ "</productType>" ++
    "</body></req>"


--subscribe :: cookie -> String -> IO ()
--subscribe cookie instrumentId =
--  let subscribe_message = "<req><body><subscription><ob2>" ++ instrumentId ++ "</ob2></subscription></body></req>"
--  request <- parseUrl "https://web-order.london-demo.lmax.com/secure/subscribe"
--  let req = request { method = methodPost
--                    , requestHeaders = [ (hContentType, BC.pack("application/xml")) ]
--                    , requestBody = RequestBodyBS (BC.pack(subscribe_message))
--                    , cookieJar = Just $ createCookieJar [cookie] }
--  res <- withManager $ httpLbs req
--  print $ responseBody res

login = do
  request <- parseUrl "https://web-order.london-demo.lmax.com/public/security/login"
  let req = request { method = methodPost
                    , requestHeaders = [ (hContentType, BC.pack("application/xml")) ]
                    , requestBody = RequestBodyBS (BC.pack(format_login "barkerm" "P4ssword." "CFD_DEMO"))
                    }
  res <- withManager $ httpLbs req
  let cookie = head $ destroyCookieJar $ responseCookieJar res
  print cookie
  print $ responseBody res
  getInstruments cookie

getInstruments cookie = do
  request' <- parseUrl "https://web-order.london-demo.lmax.com/secure/instrument/searchCurrentInstruments?q=&offset=0"
  let request = request' { requestHeaders = [ (hAccept, BC.pack("text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8")) ]
                         , cookieJar = Just $ createCookieJar [cookie]
                         }
  res <- withManager $ httpLbs request
  print $ responseBody res

main :: IO ()
main = withSocketsDo $ do
  login
--main = simpleHttp "https://web-order.london-demo.lmax.com/secure/instrument/searchCurrentInstruments?q=&offset=0" >>= L.putStr
  --request <- parseUrl "https://web-order.london-demo.lmax.com/public/security/login"
  --let req = request { method = methodPost
  --                  , requestHeaders = [ (hContentType, BC.pack("application/xml")) ]
  --                  , requestBody = RequestBodyBS (BC.pack(format_login "barkerm" "P4ssword." "CFD_DEMO"))
  --                  }
  --res <- withManager $ httpLbs req
  --let cookie = head $ destroyCookieJar $ responseCookieJar res
  --print cookie
  --print $ responseBody res
  --getInstruments cookie

  --request' <- parseUrl "https://web-order.london-demo.lmax.com/secure/instrument/searchCurrentInstruments?q=&offset=0"
  --let request = request' { requestHeaders = [ (hAccept, BC.pack("text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8")) ]
  --                       }
  --res <- withManager $ httpLbs request
  --print $ responseBody res

  --let cookie = Cookie { cookie_name = "JSESSIONID"
  --                    , cookie_value = }
  --L.putStrLn $ responseHeaders res


--login :: Connection -> String -> IO String
--login c payload = do
--    req <- buildRequest $ do
--        http POST "/public/security/login"
--        setContentType "application/xml"
--        setHostname "testapi.lmaxtrader.com"
--        setAccept "application/xml"


--main = do
--    c <- openConnection "https://testapi.lmaxtrader.com" 443