# generated from rosidl_generator_py/resource/_idl.py.em
# with input from interfaces:msg/MatWithTime.idl
# generated code does not contain a copyright notice


# Import statements for member types

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_MatWithTime(type):
    """Metaclass of message 'MatWithTime'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('interfaces')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'interfaces.msg.MatWithTime')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__mat_with_time
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__mat_with_time
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__mat_with_time
            cls._TYPE_SUPPORT = module.type_support_msg__msg__mat_with_time
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__mat_with_time

            from sensor_msgs.msg import Image
            if Image.__class__._TYPE_SUPPORT is None:
                Image.__class__.__import_type_support__()

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class MatWithTime(metaclass=Metaclass_MatWithTime):
    """Message class 'MatWithTime'."""

    __slots__ = [
        '_image',
        '_orientation',
        '_time_stamp',
        '_true_time_stamp',
    ]

    _fields_and_field_types = {
        'image': 'sensor_msgs/Image',
        'orientation': 'string',
        'time_stamp': 'double',
        'true_time_stamp': 'double',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.NamespacedType(['sensor_msgs', 'msg'], 'Image'),  # noqa: E501
        rosidl_parser.definition.UnboundedString(),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
        rosidl_parser.definition.BasicType('double'),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        from sensor_msgs.msg import Image
        self.image = kwargs.get('image', Image())
        self.orientation = kwargs.get('orientation', str())
        self.time_stamp = kwargs.get('time_stamp', float())
        self.true_time_stamp = kwargs.get('true_time_stamp', float())

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.image != other.image:
            return False
        if self.orientation != other.orientation:
            return False
        if self.time_stamp != other.time_stamp:
            return False
        if self.true_time_stamp != other.true_time_stamp:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @property
    def image(self):
        """Message field 'image'."""
        return self._image

    @image.setter
    def image(self, value):
        if __debug__:
            from sensor_msgs.msg import Image
            assert \
                isinstance(value, Image), \
                "The 'image' field must be a sub message of type 'Image'"
        self._image = value

    @property
    def orientation(self):
        """Message field 'orientation'."""
        return self._orientation

    @orientation.setter
    def orientation(self, value):
        if __debug__:
            assert \
                isinstance(value, str), \
                "The 'orientation' field must be of type 'str'"
        self._orientation = value

    @property
    def time_stamp(self):
        """Message field 'time_stamp'."""
        return self._time_stamp

    @time_stamp.setter
    def time_stamp(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'time_stamp' field must be of type 'float'"
        self._time_stamp = value

    @property
    def true_time_stamp(self):
        """Message field 'true_time_stamp'."""
        return self._true_time_stamp

    @true_time_stamp.setter
    def true_time_stamp(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'true_time_stamp' field must be of type 'float'"
        self._true_time_stamp = value
