import requests


print("Josh's Arduino Front-end Thingy\n")
USER_URL = input("Please enter the url: ")
USER_URL = "http://" + USER_URL;

r = requests.get(USER_URL).text
print(r)