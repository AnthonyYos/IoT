from twilio.rest import Client
account_sid = "AC0cf5ead72403b20c132fb03a5dd2f487"
auth_token = "ca06ec736e5347d7f37e99acdbb37216"
client = Client(account_sid, auth_token)

def sendText():
    client.messages.create(
        to="+1phonenumber",
        from_="+19782976741",
        body="This was sent from the raspberry pi"
        )
    
sendText()