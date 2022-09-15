#pragma once

enum Constants {
	InvalidSocket = -1,
	InfinityTimeout = -1,
	MsgNoFlag = 0,
	NoEvents = 0,
};

static const string& SERVER_LOG_FORMAT = "%Y-%m-%d %T";
static const string& END_OF_LINE = "\r\n";
static const string& HTTP_BORDER_LINE = "\r\n\r\n";