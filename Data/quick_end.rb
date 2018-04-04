$LINKS_HOUSE_ADDRESS = 0xE9BC
$TRIFORCE_ITEM_ID = 0x6A

data = ""
open("clean rando.sfc", "rb") {|f| data = f.read }
data[$LINKS_HOUSE_ADDRESS] = $TRIFORCE_ITEM_ID.chr
open("clean rando.sfc", "wb") {|f| f.write(data) }
