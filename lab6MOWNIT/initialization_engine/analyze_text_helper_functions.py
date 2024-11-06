import math
import re
import nltk
import scipy
from nltk.corpus import stopwords
from nltk.stem import porter
from data.paths import *
import os
import pickle


def remove_illegal_characters(original_title):
    illegal_characters = "/\\:*?\"<>|"
    title = original_title
    for char in illegal_characters:
        title = title.replace(char, "")
    return title


def get_all_words_and_titles(directory_path):
    words = []
    titles = []
    for file_name in os.listdir(directory_path):
        file_path = os.path.join(directory_path, file_name)

        if os.path.isfile(file_path):
            with open(file_path, 'r', encoding='utf-8') as file:
                try:
                    content = file.read()
                    content_words = split_into_words(text=[content])
                    words.append( content_words)
                    title = os.path.splitext(file_name)[0]
                    titles.append(title)
                except UnicodeDecodeError:
                    pass

    return words, titles

def read_article(filename):
    file_path = os.path.join(TEXTS_PATH, filename)
    with open(file_path, mode="rb") as file:
        article = pickle.load(file)
    return article

def split_into_words(text):
    result = []
    for line in text:
        for word in re.split(r'[  \t\n\r\v\f,.!:;\-=/\\()\[\]\'"]', line):
            if len(word) > 1 and word.isalpha():
                result.append(word.lower())
    return result


def stem_words(words):
    stemmer = porter.PorterStemmer()
    return [stemmer.stem(word) for word in words]


def remove_stopwords(words):
    stop_words = set(stopwords.words('english'))
    return [word for word in words if word not in stop_words]


def preprocess_words(document_words):
    #nltk.download('stopwords')
    result = []
    for document in document_words:
        document = remove_stopwords(document)
        document = stem_words(document)

        result.append(document)
    return result


def calculate_global_term_frequency(document_words):
    map = {}
    for document in document_words:
        for word in document:
            counter = map.get(word)
            map[word] = 1 if counter is None else counter+1
    result = []
    for word in map.keys():
        result.append((word, map[word]))
    result.sort(key=lambda record: record[1], reverse=True)
    return result


def calculate_term_frequency(document_words):
    result = []
    for document in document_words:
        tf = {}
        number_of_words = len(document)
        for word in document:
                counter = tf.get(word)
                tf[word] = 1/number_of_words if counter is None else counter + 1/number_of_words
        result.append(tf)
    return result


def calculate_document_frequency(document_words, vocabulary):
    df = {}
    for word in vocabulary.keys():
        occurences = 0
        for document in document_words:
            if word in document:
                occurences += 1
        df[word] = occurences
    return df


def calculate_inverse_document_frequency(document_frequency):
    idf = {}
    number_of_documents = len(document_frequency)
    for word in document_frequency.keys():
        df = document_frequency[word]
        idf[word] = math.log(number_of_documents / (df + 1))
    return idf


def calculate_tf_idf(term_frequency, inverse_document_frequency, document_words):
    result = []
    for i in range(len(term_frequency)):
        document_term_frequency = term_frequency[i]
        document_tf_idf = {}
        for term in document_words[i]:
            if term in inverse_document_frequency:
                document_tf_idf[term] = document_term_frequency[term] * inverse_document_frequency[term]

        result.append(document_tf_idf)
    return result


def tf_idf_into_matrix(tf_idf, vocabulary):
    vocab_size = len(vocabulary)
    number_of_documents = len(tf_idf)
    matrix = scipy.sparse.dok_matrix((vocab_size, number_of_documents), dtype=float)
    for i in range(number_of_documents):
        vector_squared_length = 0
        for word in tf_idf[i]:
            if word in vocabulary:
                matrix[vocabulary[word], i] = tf_idf[i][word]
                vector_squared_length += tf_idf[i][word] ** 2

        vector_length = vector_squared_length ** 0.5
        if vector_length != 0:
            for word in tf_idf[i]:
                if word in vocabulary:
                    matrix[vocabulary[word], i] /= vector_length
    return matrix