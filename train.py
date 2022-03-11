import os

import numpy as np
import tensorflow as tf
from tflite_model_maker import model_spec
from tflite_model_maker import object_detector
from tflite_model_maker.config import ExportFormat
from absl import logging


assert tf.__version__.startswith('2')


def main():
    tf.get_logger().setLevel('ERROR')
    logging.set_verbosity(logging.ERROR)

    print('===load spec')
    spec = model_spec.get('efficientdet_lite0')

    print('====load-data')
    train_data, validation_data, test_data = object_detector.DataLoader.from_csv('./labels.csv')
    print(f'train:[{len(train_data)}], validation:[{len(validation_data)}], test: [{len(test_data)}]')

    print('====train')
    model = object_detector.create(train_data, model_spec=spec, batch_size=8, train_whole_model=True, validation_data=validation_data)

    print('====eva')
    model.evaluate(test_data)
    model.export(export_dir='./model_data')

    model.evaluate_tflite('./model_data/model.tflite', test_data)


if __name__ == '__main__':
    main()
