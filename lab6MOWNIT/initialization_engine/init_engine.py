import numpy as np
from sklearn.decomposition import TruncatedSVD
from analyze_text_helper_functions import *
import csv 
import sys 
sys.path.append(os.path.join(os.path.dirname(__file__), '../data'))
from paths import *

def create_vocabulary(preprocessed_words, vocabulary_size):
    global_term_frequency = calculate_global_term_frequency(preprocessed_words)

    vocabulary = {}
    for i in range(vocabulary_size):
        word, _ = global_term_frequency[i]
        vocabulary[word] = i
    return vocabulary


def get_idf(preprocessed_words, vocabulary):
    document_frequency = calculate_document_frequency(preprocessed_words, vocabulary)
    inverse_document_frequency = calculate_inverse_document_frequency(document_frequency)
    return inverse_document_frequency


def generate_search_matrix(preprocessed_words, vocabulary, inverse_document_frequency):
    term_frequency = calculate_term_frequency(preprocessed_words)

    tf_idf = calculate_tf_idf(term_frequency, inverse_document_frequency, preprocessed_words)
    search_matrix = tf_idf_into_matrix(tf_idf, vocabulary)
    print(search_matrix)
    return search_matrix.tocoo()


def initialize_search_engine(documents_directory_path, vocabulary_size,
                             matrix_save_as_path, vocabulary_save_as_path,
                             inverse_document_frequency_save_as_path, filenames_save_as_path):
    #calculations
    print("initialization started.")

    document_words, filenames = get_all_words_and_titles(documents_directory_path)
    print("Reading documents.")

    preprocessed_words = preprocess_words(document_words)
    print("preprocessing.")

    vocabulary = create_vocabulary(preprocessed_words, vocabulary_size)
    print("Creating vocabulary.")

    inverse_document_frequency = get_idf(preprocessed_words, vocabulary)
    print(inverse_document_frequency)
    print("Calculating IDF.")

    search_matrix = generate_search_matrix(preprocessed_words, vocabulary, inverse_document_frequency)
    print("Generating search matrix.")


    #SAVINGS 
    scipy.sparse.save_npz(matrix_save_as_path, search_matrix.tobsr())

    save_dict_as_text(vocabulary, vocabulary_save_as_path)
    save_dict_as_text(inverse_document_frequency, inverse_document_frequency_save_as_path)
    save_list_as_text(filenames, filenames_save_as_path)

    print("All components saved successfully.")

def save_list_as_text(data_list, file_path):
    with open(file_path, 'w', newline='', encoding='utf-8') as file:
        writer = csv.writer(file)
        for item in data_list:
            writer.writerow([item])

def save_dict_as_text(data_dict, file_path):
    with open(file_path, 'w', newline='', encoding='utf-8') as file:
        writer = csv.writer(file)
        for key, value in data_dict.items():
            writer.writerow([key, value])


def initialize_svd(search_matrix_path, us_save_as_path, v_t_save_as_path, k=100):
    search_matrix = scipy.sparse.load_npz(search_matrix_path)
    print(search_matrix)
    print("SVD calculation.")
    svd = TruncatedSVD(n_components=k)
    svd.fit(search_matrix)

    us_matrix = svd.transform(search_matrix)
    v_t_matrix = np.array(svd.components_)
    print("SVD calculation.")

    with open(us_save_as_path, 'wb') as file:
        np.save(file, us_matrix)
    with open(v_t_save_as_path, 'wb') as file:
        np.save(file, v_t_matrix)
    print("SVD matrices saved.")


def print_first_n(dictionary, n):
    print({key: dictionary[key] for key in list(dictionary.keys())[:n]})


def init(vocabulary_size):
    initialize_search_engine(
        documents_directory_path=TEXTS_PATH,
        vocabulary_size=vocabulary_size,
        matrix_save_as_path=SEARCH_MATRIX_PATH,
        vocabulary_save_as_path=VOCABULARY_PATH,
        inverse_document_frequency_save_as_path=IDF_PATH,
        filenames_save_as_path=FILENAMES_PATH
    )


def init_svd(k): 
    initialize_svd(
        search_matrix_path=SEARCH_MATRIX_PATH,
        us_save_as_path= US_MATRIX_PATH,
        v_t_save_as_path=VT_MATRIX_PATH,
        k=k
    )

def get_necessery_data():
    init(vocabulary_size=3000)
    init_svd(k=100)
