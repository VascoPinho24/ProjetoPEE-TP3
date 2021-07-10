import json


class MySet:
    def __init__(self, action_set):
        self.action_set = json.loads(action_set)

    def getSet(self):
        return json.dumps(self.action_set)

    def getActuator(self, actuator_name):
        return json.dumps(self.action_set[actuator_name])

    def addActuactor(self, new_actuator):
        self.action_set.update(new_actuator)

    def addRule(self, actuator_name, sensor, rule_type, value, new_action):
        rule_id=1
        for rule in self.action_set[actuator_name]["rules"]:
            rule_id+=1
        rule_name = "rule " + str(rule_id)
        new_rule = {rule_name : {"type" : rule_type, "sensor": sensor, "value" : value, "action": new_action }}
        self.action_set[actuator_name]["rules"].update(new_rule)

    def updateRule(self, actuator_name, rule_id, new_type, new_sensor, new_value, new_action):
        rule = self.action_set[actuator_name]["rules"][rule_id]
        rule["type"] = new_type
        rule["sensor"] = new_sensor
        rule["value"] = new_value
        rule["action"] = new_action

    def getAction(self, sensor_name, sensor_value):
        actList = {}
        for actuator_name in self.action_set:
            for rule_name in self.action_set[actuator_name]["rules"]:
                rule = self.action_set[actuator_name]["rules"][rule_name]
                if rule["sensor"] == sensor_name:
                    if rule["type"] == "lower" and rule["value"] > sensor_value:
                        if actuator_name not in actList.keys():
                            actList[actuator_name] = []
                        actList[actuator_name].append(rule["action"])
                    if rule["type"] == "higher" and rule["value"] < sensor_value:
                        if actuator_name not in actList.keys():
                            actList[actuator_name] = []
                        actList[actuator_name].append(rule["action"])
                    if rule["type"] == "equal" and rule["value"] == sensor_value:
                        if actuator_name not in actList.keys():
                            actList[actuator_name] = []
                        actList[actuator_name].append(rule["action"])
        return actList
