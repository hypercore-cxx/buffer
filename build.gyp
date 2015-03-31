{
  'includes': [ 'common.gypi' ],
  'targets': [
    {
      'target_name': 'nodeuv-buffer',
      'product_name': 'nodeuv-buffer',
      'type': 'static_library',
      'sources': [
        'buffer.h',
        './src/buffer.cc'
      ],
      'dependencies': [
        './deps/libuv/uv.gyp:libuv',
      ],
    },
    {
      'target_name': 'test',
      'type': 'executable',
      'sources': [
        './src/buffer.cc',
        './test/test.cc',
      ],
      'dependencies': [
        './deps/libuv/uv.gyp:libuv',
        'nodeuv-buffer',
      ],
    }
  ]
}
