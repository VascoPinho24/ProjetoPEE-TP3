from MySet import *
import paho.mqtt.client as mqtt

dict = '{"Cooler" : {"type" : "actuator", "rules" : {"rule 1" : {"type" : "higher", "sensor" : 2, ' \
	       '"value" : 32, "action" : "ON"}} }}'

mySet = MySet(dict)

def mqtt_callback(client, userdata, msg):
	ruleset(client,msg)

def ruleset(client,msg):
	try:
		topic = msg.topic.split('/')[1]
	except:
		return
	if topic == "nerd":
		return

	print("Sensor " + str(topic) + " value: " + str(msg.payload))

	action = mySet.getAction(int(topic),int(msg.payload))
	for key in action:
		client.publish(key,str(action[key]))
		print("Published in " + key + " message " + str(action[key]))

def main():

	client = mqtt.Client()
	client.on_message = mqtt_callback

	client.connect("127.0.0.1",1883,60)

	client.subscribe("#",0)

	while 1:
		client.loop()

if __name__== "__main__" :
	main()

