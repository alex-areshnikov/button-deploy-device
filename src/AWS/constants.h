#ifndef aws_constants_h
#define aws_constants_h

// The name of the device. This MUST match up with the name defined in the AWS console
#define AWS_IOT_DEVICE_NAME "DeployButton"

// The MQTTT endpoint for the device (unique for each AWS account but shared amongst devices within the account)
#define AWS_IOT_ENDPOINT "a1nehtglb59qw6-ats.iot.us-west-2.amazonaws.com"

#define AWS_IOT_UPDATE_TOPIC "$aws/things/" AWS_IOT_DEVICE_NAME "/shadow/update"
#define AWS_IOT_UPDATE_DOCUMENTS_TOPIC "$aws/things/" AWS_IOT_DEVICE_NAME "/shadow/update/documents"

// How many times we should attempt to connect to AWS
#define AWS_MAX_RECONNECT_TRIES 5

#define AWS_WAKEUP_INTERVAL_MS 50

#define AWS_IOT_CURRENT_KEY "current"
#define AWS_IOT_STATE_KEY "state"
#define AWS_IOT_REPORTED_KEY "reported"
#define AWS_IOT_DESIRED_KEY "desired"

#endif