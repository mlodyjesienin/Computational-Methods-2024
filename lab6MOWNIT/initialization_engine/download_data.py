from bs4 import BeautifulSoup
import requests
import urllib.parse
import time
import os
import re
import sys 
sys.path.append(os.path.join(os.path.dirname(__file__), '../data'))
from paths import *
from init_engine import save_dict_as_text

def get_search_results(query, num_results=10):
    search_url = "https://www.google.com/search?q=" + urllib.parse.quote(query) + "&num=" + str(num_results)
    headers = {
        'User-Agent': 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:89.0) Gecko/20100101 Firefox/89.0'
    }
    
    response = requests.get(search_url, headers=headers)
    if response.status_code != 200:
        print(f"Failed to retrieve search results: {response.status_code}")
        return []
    
    
    soup = BeautifulSoup(response.text, 'html.parser')

    links = []
    for g in soup.find_all('div', class_='g'):
        a_tag = g.find('a', href=True)
        if a_tag:
            href = a_tag['href']
            if "url?q=" in href:
                url = href.split("url?q=")[1].split("&")[0]
                if url.startswith('http'):
                    links.append(url)
            elif href.startswith('/url?q='):
                url = href.split('/url?q=')[1].split('&')[0]
                if url.startswith('http'):
                    links.append(url)
            elif href.startswith('http'):
                links.append(href)
    
    return links

def sanitize_filename(filename):
    filename = re.sub(r'[\\/*?:"<>|]', "_", filename)
    return filename[:255]

def save_text_content(url, folder, url_map):
    headers = {
        'User-Agent': 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:89.0) Gecko/20100101 Firefox/89.0'
    }
    
    response = requests.get(url, headers=headers)
    if response.status_code != 200:
        print(f"Failed to retrieve page content: {response.status_code} for URL: {url}")
        return
    if url.lower().endswith('.pdf'):
            print(f"Skipping PDF: {url}")
            return
    soup = BeautifulSoup(response.text, 'html.parser')
    text = soup.get_text()
    title = soup.title.string if soup.title else None

    if not title:
        print(f"Skipping URL with no title: {url}")
        return
    
    title = sanitize_filename(title)
    
    filename = os.path.join(folder, title + ".txt")

    with open(filename, 'w', encoding='utf-8') as file:
        file.write(text)
    
    url_map[title] = url

def read_queries_from_file(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        queries = [line.strip() for line in file if line.strip()]
    return queries

def download_files():
    queries_file = QUERIES_PATH
    save_folder = TEXTS_PATH
    url_map = {}
    
    queries = read_queries_from_file(queries_file)
    
    if not os.path.exists(save_folder):
        os.makedirs(save_folder)


    for query in queries:
        print(f"Searching for: {query}")
        links = get_search_results(query,10)
        if not links:
            print(f"No links found for query: {query}")
        for link in links:
            print(f"Saving content from: {link}")
            save_text_content(link, save_folder, url_map)
            time.sleep(0.2)  # Increase this if necessary
            save_dict_as_text(url_map, URL_MAP_PATH)

    print(f"Saved text content of pages to {save_folder}")
    print(f"URL to filename mapping saved in {URL_MAP_PATH}")
