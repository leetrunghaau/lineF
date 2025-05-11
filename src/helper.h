#include <Arduino.h>

String converseLine(int lines[], String FB = "")
{
  String json = "{\"type\":\"line\",\"data\":{\"line\":[";
  for (int i = 0; i < 5; i++)
  {
    json += String(lines[i]);
    if (i < 4)
      json += ",";
  }
  json += "],\"f\":\"";
  json += FB;
  json += "\"}}";
  return json;
}


String converseSetLine(int lowLine[], int highLine[])
{
  String json = "{\"type\":\"setLine\",\"data\":{";

  // lowLine
  json += "\"lowLine\":[";
  for (int i = 0; i < 5; i++)
  {
    json += String(lowLine[i]);
    if (i < 4)
      json += ",";
  }
  json += "],";

  // highLine
  json += "\"highLine\":[";
  for (int i = 0; i < 5; i++)
  {
    json += String(highLine[i]);
    if (i < 4)
      json += ",";
  }
  json += "]}}";

  return json;
}

String converseCalibrated(int lowLine[], int highLine[], int line[])
{
  String json = "{\"type\":\"calibrated\",\"data\":{";

  // lowLine
  json += "\"lowLine\":[";
  for (int i = 0; i < 5; i++)
  {
    json += String(lowLine[i]);
    if (i < 4)
      json += ",";
  }
  json += "],";

  // highLine
  json += "\"highLine\":[";
  for (int i = 0; i < 5; i++)
  {
    json += String(highLine[i]);
    if (i < 4)
      json += ",";
  }
  json += "],";

  // line
  json += "\"line\":[";
  for (int i = 0; i < 5; i++)
  {
    json += String(line[i]);
    if (i < 4)
      json += ",";
  }
  json += "]}}";

  return json;
}
String converseConnected(int lowLine[], int highLine[], int line[])
{
  String json = "{\"type\":\"connect\",\"data\":{";

  // lowLine
  json += "\"lowLine\":[";
  for (int i = 0; i < 5; i++)
  {
    json += String(lowLine[i]);
    if (i < 4)
      json += ",";
  }
  json += "],";

  // highLine
  json += "\"highLine\":[";
  for (int i = 0; i < 5; i++)
  {
    json += String(highLine[i]);
    if (i < 4)
      json += ",";
  }
  json += "],";

  // line
  json += "\"line\":[";
  for (int i = 0; i < 5; i++)
  {
    json += String(line[i]);
    if (i < 4)
      json += ",";
  }
  json += "]}}";

  return json;
}
