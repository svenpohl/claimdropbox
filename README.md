# claimdropbox
SmartContract for ClaimDrops for WAX and AtomicAssets NFT's

*Deployed on WAX-Mainnet:*
https://wax.bloks.io/account/claimdropbox?loadContract=true&tab=Tables&account=claimdropbox&scope=claimdropbox&limit=100


# example-usage
xusotestacc1 wants do claimdrop his NFT with assetid #1099558621293 to the account xusotestacc1

transfer Asset [1099558621293] with the memo "xusotestacc1"


*You also can transfer multiple NFT's to the claimdropbox-contract:*

xusotestacc1 wants do claimdrop his NFT with assetids #1099558621293,1099558621294,1099558621295 to the accounts xusotestacc1,xusotestacc3 and xusotestacc4 
transfer Asset [1099558621293,1099558621294,1099558621295] with the memo "xusotestacc1,xusotestacc3,xusotestacc4"

*Cancel a drop*

You can cancel a drop, if a user does not claim it at any time
1) go to claimdropbox-account to contract tab, select table "drops" and find the id ( id(key) ) where you are the sender
2) go to Actions-tab, select action "canceldrop", fill your accountname to the field "sender" and the id to the field "dropid" and submit - the asset will be send back to your account, the claimdrop-entry will be removed.



DISCLAIMER:
Usage of your OWN RISK - you may deploy your own claimdropbox-contract to get sure, you have the full controll of your claim-drops.


