import os
import numpy as np
import csv
import sys
sys.path.append(os.path.join(os.path.dirname(__file__), './data'))
from paths import *
sys.path.append(os.path.join(os.path.dirname(__file__), './initialization_engine'))
from init_engine import *
from analyze_text_helper_functions import *
from download_data import download_files

def csv_to_dict_float(file_path):
    result_dict = {}
    with open(file_path, mode='r', encoding='utf-8') as file:
        reader = csv.reader(file)
        for row in reader:
            key, value = row
            result_dict[key] = float(value)
    return result_dict

def csv_to_dict_str(file_path):
    result_dict = {}
    with open(file_path, mode='r', encoding='utf-8') as file:
        reader = csv.reader(file)
        for row in reader:
            key, value = row
            result_dict[key] = value
    return result_dict

def csv_to_list(file_path):
    result_list = []
    with open(file_path, mode='r', encoding='utf-8') as file:
        reader = csv.reader(file)
        for row in reader:
            result_list.append(row)
    return result_list 

def calculate_search_vector(tf_idf,vocabulary):
        vocab_size = len(vocabulary)
        vector = np.zeros(shape=vocab_size, dtype=float)
        length_squared = 0
        for word in tf_idf:
            if word in vocabulary:
                vector[int(vocabulary[word])] = tf_idf[word]
                length_squared += tf_idf[word] ** 2

        vector_length = length_squared ** 0.5
        if vector_length != 0:
            for word in tf_idf:
                if word in vocabulary:
                    vector[int(vocabulary[word])] /= vector_length
        return vector

def search(query, SVD_FLAG = False, results_num = 10):
    # NO SVD search
    if not SVD_FLAG:
        search_matrix = scipy.sparse.load_npz(SEARCH_MATRIX_PATH).tocsr()
        number_of_documents = search_matrix.shape[1]
    
    # SVD search
    else:
        us_matrix = np.load(US_MATRIX_PATH)
        v_t_matrix = np.load(VT_MATRIX_PATH)
        number_of_documents = v_t_matrix.shape[1]

    vocabulary = csv_to_dict_float(VOCABULARY_PATH)
    titles = csv_to_list(FILENAMES_PATH)
    inverse_document_frequency = csv_to_dict_float(IDF_PATH)
    search_words = split_into_words([query])
    search_words = preprocess_words([search_words])
    term_frequency = calculate_term_frequency(search_words)
    tf_idf = calculate_tf_idf(term_frequency, inverse_document_frequency, search_words)
    search_vector = calculate_search_vector(tf_idf[0],vocabulary)
    url_dict = csv_to_dict_str(URL_MAP_PATH)

    # No SVD search
    if not SVD_FLAG:
        scores = np.array(search_vector) @ search_matrix

    # SVD search
    else:
        scores = (np.array(search_vector) @ us_matrix) @ v_t_matrix


    results = [(i, scores[i]) for i in range(number_of_documents)]

    results.sort(key=lambda x: x[1], reverse=True)
    search_results = []
    for idx, similarity in results[:results_num]:
                title = titles[idx][0]
                url = url_dict[title]
                similarity = 10 ** 2 * similarity 
                search_results.append((title,  url, similarity))
    print_results(search_results)
    return search_results

def print_results(search_results):
    i = 1
    print()
    print("RESULTS: ")
    for title,url,similairity in search_results:
        print(f"{i}.  {title}, similairity: {similairity} \n url {url} \n \n")
        i+=1

def check_file(file_path):
    if not os.path.exists(file_path):
        return False
    elif os.path.getsize(file_path) == 0:
        return False
    else:
        return True 
    
def is_directory_empty(directory_path):
    if not os.path.exists(directory_path):
        return True
    elif not os.path.isdir(directory_path):
        return True
    elif len(os.listdir(directory_path)) == 0:
        return True 
    else:
        return False 
    
def check_integrity():
     if is_directory_empty(TEXTS_PATH):
        if not check_file(QUERIES_PATH):
            print("there is no data and no queries for engine...")
            return
        download_files()
        get_necessery_data()
        return 
     
     paths = [QUERIES_PATH, FILENAMES_PATH, IDF_PATH,SEARCH_MATRIX_PATH,US_MATRIX_PATH,VT_MATRIX_PATH,URL_MAP_PATH,VOCABULARY_PATH]
     for path in paths:
        if not check_file(path):
             get_necessery_data()
             return 

#check_integrity()
query = input("\n QUERY: ")
search(query, True)       

#download_files()
#get_necessery_data()
