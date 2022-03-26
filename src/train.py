import tensorflow as tf
from absl import logging
from tflite_model_maker import model_spec
from tflite_model_maker import object_detector
from tflite_model_maker.config import ExportFormat
from tflite_model_maker.config import QuantizationConfig

assert tf.__version__.startswith('2')


def main():
    tf.get_logger().setLevel('ERROR')
    logging.set_verbosity(logging.ERROR)

    print('===load spec')
    spec = model_spec.get('efficientdet_lite2')
    spec.config.var_freeze_expr = 'efficientnet'

    print('====load-data')
    train_data, validation_data, test_data = object_detector.DataLoader.from_csv('./labels.csv')
    print(f'train:[{len(train_data)}], validation:[{len(validation_data)}], test: [{len(test_data)}]')

    print('====train')
    model = object_detector.create(train_data, model_spec=spec, epochs=50, batch_size=8, train_whole_model=True,
                                   validation_data=validation_data)

    print('====eva')
    model.evaluate(test_data)

    print('====exp')
    # config = QuantizationConfig.for_float16()
    model.export(export_dir='../model_data',
                 # export_format=[ExportFormat.TFLITE, ExportFormat.SAVED_MODEL, ExportFormat.LABEL],
                 # quantization_config=config
                 )

    print('====evaluate_tflite')
    model.evaluate_tflite('./model_data/model.tflite', test_data)


if __name__ == '__main__':
    main()
