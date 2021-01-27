import md5 from '@lib/md5'
/* global XLSX */
// import { dp } from '@lib/util'

// id, name, value
const properties = []
// id, name, state, description
const templates = [
  {
    description: '基本类型设计',
    id: 1,
    name: '基础模板',
    state: 0
  }
]
// id, template_id, no, name, type, description
const types = [
  {
    description: '单行文本',
    id: 1,
    name: '文本',
    no: 1,
    template_id: null,
    type: 0
  },
  {
    description: '可换行文本',
    id: 2,
    name: '长文本',
    no: 2,
    template_id: null,
    type: 0
  },
  {
    description: '整数或者实数、可以规定小数位数',
    id: 3,
    name: '数字',
    no: 3,
    template_id: null,
    type: 0
  },
  {
    description: '整数',
    id: 4,
    name: '整数',
    no: 4,
    template_id: null,
    type: 0
  },
  {
    description: '实数',
    id: 5,
    name: '实数',
    no: 5,
    template_id: null,
    type: 0
  },
  {
    description: '年月日',
    id: 6,
    name: '日期',
    no: 6,
    template_id: null,
    type: 0
  },
  {
    description: '时分秒',
    id: 7,
    name: '时间',
    no: 7,
    template_id: null,
    type: 0
  },
  {
    description: '二进制内容',
    id: 8,
    name: '数据对象',
    no: 8,
    template_id: null,
    type: 0
  },
  {
    description: '图片内容',
    id: 9,
    name: '图片',
    no: 9,
    template_id: null,
    type: 0
  },
  {
    description: '所有的配置',
    id: 10,
    name: 'properties',
    no: 65,
    template_id: null,
    type: 3
  },
  {
    description: '所有的模板',
    id: 11,
    name: 'templates',
    no: 66,
    template_id: null,
    type: 3
  },
  {
    description: '所有的类型',
    id: 12,
    name: 'types',
    no: 67,
    template_id: null,
    type: 3
  },
  {
    description: '所有的限制',
    id: 13,
    name: 'restrictions',
    no: 68,
    template_id: null,
    type: 3
  },
  {
    description: '所有的单位范畴',
    id: 14,
    name: 'unit_types',
    no: 69,
    template_id: null,
    type: 3
  },
  {
    description: '所有的单位',
    id: 15,
    name: 'units',
    no: 70,
    template_id: null,
    type: 3
  },
  {
    description: '所有的信息组织',
    id: 16,
    name: 'info_configs',
    no: 71,
    template_id: null,
    type: 3
  },
  {
    description: '所有的枚举',
    id: 17,
    name: 'codes',
    no: 72,
    template_id: null,
    type: 3
  },
  {
    description: '所有的复合',
    id: 18,
    name: 'schemas',
    no: 73,
    template_id: null,
    type: 3
  },
  {
    description: '所有的列表',
    id: 19,
    name: 'lists',
    no: 74,
    template_id: null,
    type: 3
  },
  {
    description: '所有的基线',
    id: 20,
    name: 'baselines',
    no: 75,
    template_id: null,
    type: 3
  },
  {
    description: '所有的数据',
    id: 21,
    name: 'data',
    no: 76,
    template_id: null,
    type: 3
  },
  {
    description: '所有的文件',
    id: 22,
    name: 'files',
    no: 77,
    template_id: null,
    type: 3
  },
  {
    description: '所有的组织',
    id: 23,
    name: 'organizations',
    no: 78,
    template_id: null,
    type: 3
  },
  {
    description: '所有的用户',
    id: 24,
    name: 'users',
    no: 79,
    template_id: null,
    type: 3
  },
  {
    description: '所有的日志',
    id: 25,
    name: 'logs',
    no: 80,
    template_id: null,
    type: 3
  },
  {
    description: '所有的机型',
    id: 26,
    name: 'models',
    no: 81,
    template_id: null,
    type: 3
  },
  {
    description: '所有的状态',
    id: 27,
    name: 'states',
    no: 82,
    template_id: null,
    type: 3
  },
  {
    description: '所有的批次',
    id: 28,
    name: 'batchs',
    no: 83,
    template_id: null,
    type: 3
  },
  {
    description: '所有的飞机',
    id: 29,
    name: 'airs',
    no: 84,
    template_id: null,
    type: 3
  },
  {
    description: '所有的变更',
    id: 30,
    name: 'histories',
    no: 85,
    template_id: null,
    type: 3
  },
  {
    description: '配置的属性',
    id: 31,
    name: 'properties项类型',
    no: 129,
    template_id: null,
    type: 3
  },
  {
    description: '模板的属性',
    id: 32,
    name: 'templates项类型',
    no: 120,
    template_id: null,
    type: 3
  },
  {
    description: '类型的属性',
    id: 33,
    name: 'types项类型',
    no: 131,
    template_id: null,
    type: 3
  },
  {
    description: '限制的属性',
    id: 34,
    name: 'restrictions项类型',
    no: 132,
    template_id: null,
    type: 3
  },
  {
    description: '单位范畴的属性',
    id: 35,
    name: 'unit_types项类型',
    no: 133,
    template_id: null,
    type: 3
  },
  {
    description: '单位的属性',
    id: 36,
    name: 'units项类型',
    no: 134,
    template_id: null,
    type: 3
  },
  {
    description: '信息组织的属性',
    id: 37,
    name: 'info_configs项类型',
    no: 135,
    template_id: null,
    type: 3
  },
  {
    description: '枚举的属性',
    id: 38,
    name: 'codes项类型',
    no: 136,
    template_id: null,
    type: 3
  },
  {
    description: '复合的属性',
    id: 39,
    name: 'schemas项类型',
    no: 137,
    template_id: null,
    type: 3
  },
  {
    description: '列表的属性',
    id: 40,
    name: 'lists项类型',
    no: 138,
    template_id: null,
    type: 3
  },
  {
    description: '基线的属性',
    id: 41,
    name: 'baselines项类型',
    no: 139,
    template_id: null,
    type: 3
  },
  {
    description: '数据的属性',
    id: 42,
    name: 'data项类型',
    no: 140,
    template_id: null,
    type: 3
  },
  {
    description: '文件的属性',
    id: 43,
    name: 'files项类型',
    no: 141,
    template_id: null,
    type: 3
  },
  {
    description: '单位的属性',
    id: 44,
    name: 'organizations项类型',
    no: 142,
    template_id: null,
    type: 2
  },
  {
    description: '用户的属性',
    id: 45,
    name: 'users项类型',
    no: 143,
    template_id: null,
    type: 2
  },
  {
    description: '日志的属性',
    id: 46,
    name: 'logs项类型',
    no: 144,
    template_id: null,
    type: 2
  },
  {
    description: '机型的属性',
    id: 47,
    name: 'models项类型',
    no: 145,
    template_id: null,
    type: 2
  },
  {
    description: '状态的属性',
    id: 48,
    name: 'states项类型',
    no: 146,
    template_id: null,
    type: 2
  },
  {
    description: '批次的属性',
    id: 49,
    name: 'batchs项类型',
    no: 147,
    template_id: null,
    type: 2
  },
  {
    description: '飞机的属性',
    id: 50,
    name: 'airs项类型',
    no: 148,
    template_id: null,
    type: 2
  },
  {
    description: '变更的属性',
    id: 51,
    name: 'histories项类型',
    no: 149,
    template_id: null,
    type: 2
  },
  {
    description: null,
    id: 52,
    name: 'types__type',
    no: 257,
    template_id: null,
    type: 1
  },
  {
    description: null,
    id: 53,
    name: 'info_configs__type',
    no: 258,
    template_id: null,
    type: 1
  },
  {
    description: null,
    id: 54,
    name: 'schemas__is_required',
    no: 259,
    template_id: null,
    type: 1
  },
  {
    description: null,
    id: 55,
    name: 'schemas__is_hidden',
    no: 260,
    template_id: null,
    type: 1
  },
  {
    description: null,
    id: 56,
    name: 'baselines__object_type',
    no: 261,
    template_id: null,
    type: 1
  },
  {
    description: null,
    id: 57,
    name: 'organizations__type',
    no: 262,
    template_id: 1,
    type: 1
  },
  {
    description: null,
    id: 58,
    name: 'users__sex',
    no: 263,
    template_id: 1,
    type: 1
  },
  {
    description: null,
    id: 59,
    name: 'models__type',
    no: 264,
    template_id: 1,
    type: 1
  }
]
// id, template_id, length, format, precision, min, max
const restrictions = []
// id, name, description
const unitTypes = [
  {
    description: null,
    id: 1,
    name: '量词'
  },
  {
    description: null,
    id: 2,
    name: '长度'
  },
  {
    description: null,
    id: 3,
    name: '面积'
  },
  {
    description: null,
    id: 4,
    name: '体积'
  },
  {
    description: null,
    id: 5,
    name: '质量'
  },
  {
    description: null,
    id: 6,
    name: '时间'
  },
  {
    description: null,
    id: 7,
    name: '平面角'
  },
  {
    description: null,
    id: 8,
    name: '密度'
  },
  {
    description: null,
    id: 9,
    name: '压力'
  },
  {
    description: null,
    id: 10,
    name: '力'
  },
  {
    description: null,
    id: 11,
    name: '温度'
  },
  {
    description: null,
    id: 12,
    name: '传导系数'
  },
  {
    description: null,
    id: 13,
    name: '比热容'
  },
  {
    description: null,
    id: 14,
    name: '能量、功、热'
  },
  {
    description: null,
    id: 15,
    name: '功率、辐射能量'
  },
  {
    description: null,
    id: 16,
    name: '速度'
  },
  {
    description: null,
    id: 17,
    name: '电容'
  },
  {
    description: null,
    id: 18,
    name: '电阻'
  },
  {
    description: null,
    id: 19,
    name: '流量'
  },
  {
    description: null,
    id: 20,
    name: '频率'
  },
  {
    description: null,
    id: 21,
    name: '电荷量'
  },
  {
    description: null,
    id: 22,
    name: '电势、电压、电动势'
  },
  {
    description: null,
    id: 23,
    name: '电导'
  },
  {
    description: null,
    id: 24,
    name: '磁通量'
  },
  {
    description: null,
    id: 25,
    name: '磁通量密度'
  },
  {
    description: null,
    id: 26,
    name: '电感'
  },
  {
    description: null,
    id: 27,
    name: '光通量'
  },
  {
    description: null,
    id: 28,
    name: '光照度'
  },
  {
    description: null,
    id: 29,
    name: '放射性活度'
  },
  {
    description: null,
    id: 30,
    name: '吸收当量'
  },
  {
    description: null,
    id: 31,
    name: '剂量当量'
  },
  {
    description: null,
    id: 32,
    name: '旋转速度'
  },
  {
    description: null,
    id: 33,
    name: '能'
  },
  {
    description: null,
    id: 34,
    name: '级差'
  },
  {
    description: null,
    id: 35,
    name: '线密度'
  }
]
// id, type_id, name, symbol, description
const units = [
  {
    description: null,
    id: 1,
    name: '个',
    symbol: '个',
    type_id: 1
  },
  {
    description: null,
    id: 2,
    name: '件',
    symbol: '件',
    type_id: 1
  },
  {
    description: null,
    id: 3,
    name: '辆',
    symbol: '辆',
    type_id: 1
  },
  {
    description: null,
    id: 4,
    name: '架',
    symbol: '架',
    type_id: 1
  },
  {
    description: null,
    id: 5,
    name: '台',
    symbol: '台',
    type_id: 1
  },
  {
    description: null,
    id: 6,
    name: '片',
    symbol: '片',
    type_id: 1
  },
  {
    description: null,
    id: 7,
    name: '颗',
    symbol: '颗',
    type_id: 1
  },
  {
    description: null,
    id: 8,
    name: '粒',
    symbol: '粒',
    type_id: 1
  },
  {
    description: null,
    id: 9,
    name: '门',
    symbol: '门',
    type_id: 1
  },
  {
    description: null,
    id: 10,
    name: '只',
    symbol: '只',
    type_id: 1
  },
  {
    description: null,
    id: 11,
    name: '双',
    symbol: '双',
    type_id: 1
  },
  {
    description: null,
    id: 12,
    name: '棵',
    symbol: '棵',
    type_id: 1
  },
  {
    description: null,
    id: 13,
    name: '张',
    symbol: '张',
    type_id: 1
  },
  {
    description: null,
    id: 14,
    name: '串',
    symbol: '串',
    type_id: 1
  },
  {
    description: null,
    id: 15,
    name: '把',
    symbol: '把',
    type_id: 1
  },
  {
    description: null,
    id: 16,
    name: '群',
    symbol: '群',
    type_id: 1
  },
  {
    description: null,
    id: 17,
    name: '本',
    symbol: '本',
    type_id: 1
  },
  {
    description: null,
    id: 18,
    name: '匹',
    symbol: '匹',
    type_id: 1
  },
  {
    description: null,
    id: 19,
    name: '条',
    symbol: '条',
    type_id: 1
  },
  {
    description: null,
    id: 20,
    name: '部',
    symbol: '部',
    type_id: 1
  },
  {
    description: null,
    id: 21,
    name: '艘',
    symbol: '艘',
    type_id: 1
  },
  {
    description: null,
    id: 22,
    name: '间',
    symbol: '间',
    type_id: 1
  },
  {
    description: null,
    id: 23,
    name: '束',
    symbol: '束',
    type_id: 1
  },
  {
    description: null,
    id: 24,
    name: '幅',
    symbol: '幅',
    type_id: 1
  },
  {
    description: null,
    id: 25,
    name: '种',
    symbol: '种',
    type_id: 1
  },
  {
    description: null,
    id: 26,
    name: '杯',
    symbol: '杯',
    type_id: 1
  },
  {
    description: null,
    id: 27,
    name: '千米',
    symbol: 'Km',
    type_id: 2
  },
  {
    description: null,
    id: 28,
    name: '米',
    symbol: 'm',
    type_id: 2
  },
  {
    description: null,
    id: 29,
    name: '分米',
    symbol: 'dm',
    type_id: 2
  },
  {
    description: null,
    id: 30,
    name: '厘米',
    symbol: 'cm',
    type_id: 2
  },
  {
    description: null,
    id: 31,
    name: '毫米',
    symbol: 'mm',
    type_id: 2
  },
  {
    description: null,
    id: 32,
    name: '微米',
    symbol: 'um',
    type_id: 2
  },
  {
    description: null,
    id: 33,
    name: '海里',
    symbol: 'mile',
    type_id: 2
  },
  {
    description: null,
    id: 34,
    name: '平方千米',
    symbol: 'Km2',
    type_id: 3
  },
  {
    description: null,
    id: 35,
    name: '平方米',
    symbol: 'm2',
    type_id: 3
  },
  {
    description: null,
    id: 36,
    name: '平方分米',
    symbol: 'dm2',
    type_id: 3
  },
  {
    description: null,
    id: 37,
    name: '平方厘米',
    symbol: 'cm2',
    type_id: 3
  },
  {
    description: null,
    id: 38,
    name: '平方毫米',
    symbol: 'mm2',
    type_id: 3
  },
  {
    description: null,
    id: 39,
    name: '平方微米',
    symbol: 'um2',
    type_id: 3
  },
  {
    description: null,
    id: 40,
    name: '公顷',
    symbol: 'ha',
    type_id: 3
  },
  {
    description: null,
    id: 41,
    name: '立方千米',
    symbol: 'Km3',
    type_id: 4
  },
  {
    description: null,
    id: 42,
    name: '立方米',
    symbol: 'm3',
    type_id: 4
  },
  {
    description: null,
    id: 43,
    name: '立方分米',
    symbol: 'dm3',
    type_id: 4
  },
  {
    description: null,
    id: 44,
    name: '立方厘米',
    symbol: 'cm3',
    type_id: 4
  },
  {
    description: null,
    id: 45,
    name: '立方毫米',
    symbol: 'mm3',
    type_id: 4
  },
  {
    description: null,
    id: 46,
    name: '立方微米',
    symbol: 'um3',
    type_id: 4
  },
  {
    description: null,
    id: 47,
    name: '升',
    symbol: 'L',
    type_id: 4
  },
  {
    description: null,
    id: 48,
    name: '毫升',
    symbol: 'mL',
    type_id: 4
  },
  {
    description: null,
    id: 49,
    name: '吨',
    symbol: 't',
    type_id: 5
  },
  {
    description: null,
    id: 50,
    name: '千克',
    symbol: 'Kg',
    type_id: 5
  },
  {
    description: null,
    id: 51,
    name: '克',
    symbol: 'g',
    type_id: 5
  },
  {
    description: null,
    id: 52,
    name: '秒',
    symbol: 's',
    type_id: 6
  },
  {
    description: null,
    id: 53,
    name: '分',
    symbol: 'min',
    type_id: 6
  },
  {
    description: null,
    id: 54,
    name: '小时',
    symbol: 'h',
    type_id: 6
  },
  {
    description: null,
    id: 55,
    name: '秒',
    symbol: '“',
    type_id: 7
  },
  {
    description: null,
    id: 56,
    name: '分',
    symbol: '‘',
    type_id: 7
  },
  {
    description: null,
    id: 57,
    name: '度',
    symbol: '˚',
    type_id: 7
  },
  {
    description: null,
    id: 58,
    name: '千克/立方米',
    symbol: 'Kg/m3',
    type_id: 8
  },
  {
    description: null,
    id: 59,
    name: '克/立方厘米',
    symbol: 'g/cm3',
    type_id: 8
  },
  {
    description: null,
    id: 60,
    name: '兆帕',
    symbol: 'Mpa',
    type_id: 9
  },
  {
    description: null,
    id: 61,
    name: '帕',
    symbol: 'Pa',
    type_id: 9
  },
  {
    description: null,
    id: 62,
    name: '巴',
    symbol: 'b',
    type_id: 9
  },
  {
    description: null,
    id: 63,
    name: '大气压',
    symbol: 'at m',
    type_id: 9
  },
  {
    description: null,
    id: 64,
    name: '力',
    symbol: 'N',
    type_id: 10
  },
  {
    description: null,
    id: 65,
    name: '开尔文度',
    symbol: 'K',
    type_id: 11
  },
  {
    description: null,
    id: 66,
    name: '摄氏度',
    symbol: '℃',
    type_id: 11
  },
  {
    description: null,
    id: 67,
    name: '华氏度',
    symbol: '℉',
    type_id: 11
  },
  {
    description: null,
    id: 68,
    name: '千卡',
    symbol: 'kcal/m2.h.℃',
    type_id: 12
  },
  {
    description: null,
    id: 69,
    name: '千卡/千克.℃',
    symbol: 'kcal/kg.℃',
    type_id: 13
  },
  {
    description: null,
    id: 70,
    name: '焦耳',
    symbol: 'J',
    type_id: 14
  },
  {
    description: null,
    id: 71,
    name: '卡',
    symbol: 'cal',
    type_id: 14
  },
  {
    description: null,
    id: 72,
    name: '千瓦时',
    symbol: 'kW.h',
    type_id: 14
  },
  {
    description: null,
    id: 73,
    name: '千瓦',
    symbol: 'kW',
    type_id: 15
  },
  {
    description: null,
    id: 74,
    name: '瓦',
    symbol: 'W',
    type_id: 15
  },
  {
    description: null,
    id: 75,
    name: '马力',
    symbol: 'hp',
    type_id: 15
  },
  {
    description: null,
    id: 76,
    name: '千米/小时',
    symbol: 'km/h',
    type_id: 16
  },
  {
    description: null,
    id: 77,
    name: '米/秒',
    symbol: 'm/s',
    type_id: 16
  },
  {
    description: null,
    id: 78,
    name: '节',
    symbol: 'kn',
    type_id: 16
  },
  {
    description: null,
    id: 79,
    name: '法',
    symbol: 'F',
    type_id: 17
  },
  {
    description: null,
    id: 80,
    name: '微法',
    symbol: 'mF',
    type_id: 17
  },
  {
    description: null,
    id: 81,
    name: '皮法',
    symbol: 'pF',
    type_id: 17
  },
  {
    description: null,
    id: 82,
    name: '欧姆',
    symbol: 'Ω',
    type_id: 18
  },
  {
    description: null,
    id: 83,
    name: '立方米/秒',
    symbol: '(m3/s)',
    type_id: 19
  },
  {
    description: null,
    id: 84,
    name: '赫兹',
    symbol: 'Hz',
    type_id: 20
  },
  {
    description: null,
    id: 85,
    name: '库伦',
    symbol: 'C',
    type_id: 21
  },
  {
    description: null,
    id: 86,
    name: '伏特',
    symbol: 'V',
    type_id: 22
  },
  {
    description: null,
    id: 87,
    name: '西门子',
    symbol: 'S',
    type_id: 23
  },
  {
    description: null,
    id: 88,
    name: '韦伯',
    symbol: 'Wb',
    type_id: 24
  },
  {
    description: null,
    id: 89,
    name: '特斯拉',
    symbol: 'T',
    type_id: 25
  },
  {
    description: null,
    id: 90,
    name: '亨利',
    symbol: 'H',
    type_id: 26
  },
  {
    description: null,
    id: 91,
    name: '流明',
    symbol: 'lm',
    type_id: 27
  },
  {
    description: null,
    id: 92,
    name: '勒克斯',
    symbol: 'lx',
    type_id: 28
  },
  {
    description: null,
    id: 93,
    name: '贝可',
    symbol: 'Bq',
    type_id: 29
  },
  {
    description: null,
    id: 94,
    name: '戈瑞',
    symbol: 'Gy',
    type_id: 30
  },
  {
    description: null,
    id: 95,
    name: '希沃特',
    symbol: 'Sv',
    type_id: 31
  },
  {
    description: null,
    id: 96,
    name: '转/分',
    symbol: 'r/min',
    type_id: 32
  },
  {
    description: null,
    id: 97,
    name: '电子伏',
    symbol: 'eV',
    type_id: 33
  },
  {
    description: null,
    id: 98,
    name: '分贝',
    symbol: 'dB',
    type_id: 34
  },
  {
    description: null,
    id: 99,
    name: '特克斯',
    symbol: 'tex',
    type_id: 35
  }
]
// id, template_id, parent_id, type, no, name, type_no, reference_to, is_required, restriction_id, unit_id, description
const infoConfigs = []
// id, template_id, parent_no, no, value, description
const codes = [
  {
    description: null,
    id: 1,
    no: 0,
    parent_no: 257,
    template_id: null,
    value: '原始类型'
  },
  {
    description: null,
    id: 2,
    no: 1,
    parent_no: 257,
    template_id: null,
    value: '枚举类型'
  },
  {
    description: null,
    id: 3,
    no: 2,
    parent_no: 257,
    template_id: null,
    value: '复合类型'
  },
  {
    description: null,
    id: 4,
    no: 3,
    parent_no: 257,
    template_id: null,
    value: '列表类型'
  },
  {
    description: null,
    id: 5,
    no: 0,
    parent_no: 258,
    template_id: null,
    value: '数据包'
  },
  {
    description: null,
    id: 6,
    no: 1,
    parent_no: 258,
    template_id: null,
    value: '数据项'
  },
  {
    description: null,
    id: 7,
    no: 0,
    parent_no: 259,
    template_id: null,
    value: '选填'
  },
  {
    description: null,
    id: 8,
    no: 1,
    parent_no: 259,
    template_id: null,
    value: '必填'
  },
  {
    description: null,
    id: 9,
    no: 0,
    parent_no: 260,
    template_id: null,
    value: '显示'
  },
  {
    description: null,
    id: 10,
    no: 1,
    parent_no: 260,
    template_id: null,
    value: '隐藏'
  },
  {
    description: null,
    id: 11,
    no: 0,
    parent_no: 261,
    template_id: null,
    value: '批次'
  },
  {
    description: null,
    id: 12,
    no: 1,
    parent_no: 261,
    template_id: null,
    value: '单机'
  },
  {
    description: '用于设计数据类型并分析数据差异的单位',
    id: 13,
    no: 0,
    parent_no: 262,
    template_id: 1,
    value: '管理单位'
  },
  {
    description: '用于填报数据的单位',
    id: 14,
    no: 1,
    parent_no: 262,
    template_id: 1,
    value: '承制单位'
  },
  {
    description: '用于维护单机技术状态并进行状态变更的单位',
    id: 15,
    no: 2,
    parent_no: 262,
    template_id: 1,
    value: '接装单位'
  },
  {
    description: null,
    id: 16,
    no: 0,
    parent_no: 263,
    template_id: 1,
    value: '女'
  },
  {
    description: null,
    id: 17,
    no: 1,
    parent_no: 263,
    template_id: 1,
    value: '男'
  },
  {
    description: null,
    id: 18,
    no: 0,
    parent_no: 264,
    template_id: 1,
    value: '歼击机'
  },
  {
    description: null,
    id: 19,
    no: 1,
    parent_no: 264,
    template_id: 1,
    value: '强击机'
  },
  {
    description: null,
    id: 20,
    no: 2,
    parent_no: 264,
    template_id: 1,
    value: '轰炸机'
  },
  {
    description: null,
    id: 21,
    no: 3,
    parent_no: 264,
    template_id: 1,
    value: '运输机'
  },
  {
    description: null,
    id: 22,
    no: 5,
    parent_no: 264,
    template_id: 1,
    value: '直升机'
  },
  {
    description: null,
    id: 23,
    no: 6,
    parent_no: 264,
    template_id: 1,
    value: '教练机'
  },
  {
    description: null,
    id: 24,
    no: 7,
    parent_no: 264,
    template_id: 1,
    value: '特种机'
  },
  {
    description: null,
    id: 25,
    no: 8,
    parent_no: 264,
    template_id: 1,
    value: '无人机'
  }
]
// id, template_id, parent_no, no, name, title, is_hidden, type_no, is_required, restriction_id, unit_id, description
const schemas = [
  {
    description: '主键',
    id: 1,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 129,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 2,
    is_hidden: 0,
    is_required: 0,
    name: 'name',
    no: 1,
    parent_no: 129,
    restriction_id: null,
    template_id: null,
    title: '名称',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 3,
    is_hidden: 0,
    is_required: 0,
    name: 'value',
    no: 2,
    parent_no: 129,
    restriction_id: null,
    template_id: null,
    title: '值',
    type_no: 2,
    unit_id: null
  },
  {
    description: '主键',
    id: 4,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 130,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 5,
    is_hidden: 0,
    is_required: 1,
    name: 'name',
    no: 1,
    parent_no: 130,
    restriction_id: null,
    template_id: null,
    title: '名称',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 6,
    is_hidden: 1,
    is_required: 1,
    name: 'state',
    no: 2,
    parent_no: 130,
    restriction_id: null,
    template_id: null,
    title: '状态',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 7,
    is_hidden: 0,
    is_required: 0,
    name: 'description',
    no: 3,
    parent_no: 130,
    restriction_id: null,
    template_id: null,
    title: '备注',
    type_no: 2,
    unit_id: null
  },
  {
    description: '主键',
    id: 8,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 131,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 9,
    is_hidden: 0,
    is_required: 1,
    name: 'no',
    no: 1,
    parent_no: 131,
    restriction_id: null,
    template_id: null,
    title: '编号',
    type_no: 1,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "name"}',
    id: 10,
    is_hidden: 1,
    is_required: 0,
    name: 'template_id',
    no: 2,
    parent_no: 131,
    restriction_id: null,
    template_id: null,
    title: '模板id',
    type_no: 66,
    unit_id: null
  },
  {
    description: null,
    id: 11,
    is_hidden: 0,
    is_required: 1,
    name: 'name',
    no: 3,
    parent_no: 131,
    restriction_id: null,
    template_id: null,
    title: '名称',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 12,
    is_hidden: 0,
    is_required: 1,
    name: 'type',
    no: 4,
    parent_no: 131,
    restriction_id: null,
    template_id: null,
    title: '类型',
    type_no: 257,
    unit_id: null
  },
  {
    description: null,
    id: 13,
    is_hidden: 1,
    is_required: 0,
    name: 'description',
    no: 5,
    parent_no: 131,
    restriction_id: null,
    template_id: null,
    title: '备注',
    type_no: 2,
    unit_id: null
  },
  {
    description: '主键',
    id: 14,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 132,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "name"}',
    id: 15,
    is_hidden: 1,
    is_required: 1,
    name: 'template_id',
    no: 1,
    parent_no: 132,
    restriction_id: null,
    template_id: null,
    title: '模板id',
    type_no: 66,
    unit_id: null
  },
  {
    description: null,
    id: 16,
    is_hidden: 0,
    is_required: 0,
    name: 'length',
    no: 2,
    parent_no: 132,
    restriction_id: null,
    template_id: null,
    title: '长度',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 17,
    is_hidden: 0,
    is_required: 0,
    name: 'precision',
    no: 3,
    parent_no: 132,
    restriction_id: null,
    template_id: null,
    title: '精度',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 18,
    is_hidden: 0,
    is_required: 0,
    name: 'min',
    no: 4,
    parent_no: 132,
    restriction_id: null,
    template_id: null,
    title: '最小值',
    type_no: 5,
    unit_id: null
  },
  {
    description: null,
    id: 19,
    is_hidden: 1,
    is_required: 0,
    name: 'max',
    no: 5,
    parent_no: 132,
    restriction_id: null,
    template_id: null,
    title: '最大值',
    type_no: 5,
    unit_id: null
  },
  {
    description: '主键',
    id: 20,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 133,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 21,
    is_hidden: 0,
    is_required: 1,
    name: 'name',
    no: 1,
    parent_no: 133,
    restriction_id: null,
    template_id: null,
    title: '名称',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 22,
    is_hidden: 0,
    is_required: 0,
    name: 'description',
    no: 2,
    parent_no: 133,
    restriction_id: null,
    template_id: null,
    title: '备注',
    type_no: 2,
    unit_id: null
  },
  {
    description: '主键',
    id: 23,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 134,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "name"}',
    id: 24,
    is_hidden: 1,
    is_required: 0,
    name: 'type_id',
    no: 1,
    parent_no: 134,
    restriction_id: null,
    template_id: null,
    title: '范畴id',
    type_no: 69,
    unit_id: null
  },
  {
    description: null,
    id: 25,
    is_hidden: 0,
    is_required: 1,
    name: 'name',
    no: 2,
    parent_no: 134,
    restriction_id: null,
    template_id: null,
    title: '名称',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 26,
    is_hidden: 0,
    is_required: 0,
    name: 'symbol',
    no: 3,
    parent_no: 134,
    restriction_id: null,
    template_id: null,
    title: '符号',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 27,
    is_hidden: 0,
    is_required: 0,
    name: 'description',
    no: 4,
    parent_no: 134,
    restriction_id: null,
    template_id: null,
    title: '备注',
    type_no: 2,
    unit_id: null
  },
  {
    description: '主键',
    id: 28,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 135,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "name"}',
    id: 29,
    is_hidden: 1,
    is_required: 1,
    name: 'template_id',
    no: 1,
    parent_no: 135,
    restriction_id: null,
    template_id: null,
    title: '模板id',
    type_no: 4,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "name"}',
    id: 30,
    is_hidden: 0,
    is_required: 0,
    name: 'parent_id',
    no: 2,
    parent_no: 135,
    restriction_id: null,
    template_id: null,
    title: '父id',
    type_no: 71,
    unit_id: null
  },
  {
    description: null,
    id: 31,
    is_hidden: 0,
    is_required: 1,
    name: 'type',
    no: 3,
    parent_no: 135,
    restriction_id: null,
    template_id: null,
    title: '类型',
    type_no: 258,
    unit_id: null
  },
  {
    description: null,
    id: 32,
    is_hidden: 0,
    is_required: 1,
    name: 'no',
    no: 4,
    parent_no: 135,
    restriction_id: null,
    template_id: null,
    title: '序号',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 33,
    is_hidden: 0,
    is_required: 1,
    name: 'name',
    no: 5,
    parent_no: 135,
    restriction_id: null,
    template_id: null,
    title: '名称',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 34,
    is_hidden: 0,
    is_required: 1,
    name: 'type_no',
    no: 6,
    parent_no: 135,
    restriction_id: null,
    template_id: null,
    title: '类型编号',
    type_no: 4,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "name"}',
    id: 35,
    is_hidden: 0,
    is_required: 0,
    name: 'reference_to',
    no: 7,
    parent_no: 135,
    restriction_id: null,
    template_id: null,
    title: '引用到',
    type_no: 71,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "id"}',
    id: 36,
    is_hidden: 0,
    is_required: 0,
    name: 'restriction_id',
    no: 8,
    parent_no: 135,
    restriction_id: null,
    template_id: null,
    title: '限制id',
    type_no: 68,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "symbol"}',
    id: 37,
    is_hidden: 0,
    is_required: 0,
    name: 'unit_id',
    no: 9,
    parent_no: 135,
    restriction_id: null,
    template_id: null,
    title: '单位id',
    type_no: 70,
    unit_id: null
  },
  {
    description: '主键',
    id: 38,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 136,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "name"}',
    id: 39,
    is_hidden: 1,
    is_required: 1,
    name: 'template_id',
    no: 1,
    parent_no: 136,
    restriction_id: null,
    template_id: null,
    title: '模板id',
    type_no: 66,
    unit_id: null
  },
  {
    description: null,
    id: 40,
    is_hidden: 1,
    is_required: 1,
    name: 'parent_no',
    no: 2,
    parent_no: 136,
    restriction_id: null,
    template_id: null,
    title: '父编号',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 41,
    is_hidden: 0,
    is_required: 1,
    name: 'no',
    no: 3,
    parent_no: 136,
    restriction_id: null,
    template_id: null,
    title: '序号',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 42,
    is_hidden: 0,
    is_required: 1,
    name: 'value',
    no: 4,
    parent_no: 136,
    restriction_id: null,
    template_id: null,
    title: '值',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 43,
    is_hidden: 1,
    is_required: 0,
    name: 'description',
    no: 5,
    parent_no: 136,
    restriction_id: null,
    template_id: null,
    title: '备注',
    type_no: 2,
    unit_id: null
  },
  {
    description: '主键',
    id: 44,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 137,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "name"}',
    id: 45,
    is_hidden: 1,
    is_required: 1,
    name: 'template_id',
    no: 1,
    parent_no: 137,
    restriction_id: null,
    template_id: null,
    title: '模板id',
    type_no: 66,
    unit_id: null
  },
  {
    description: null,
    id: 46,
    is_hidden: 1,
    is_required: 1,
    name: 'parent_no',
    no: 2,
    parent_no: 137,
    restriction_id: null,
    template_id: null,
    title: '父编号',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 47,
    is_hidden: 1,
    is_required: 1,
    name: 'no',
    no: 3,
    parent_no: 137,
    restriction_id: null,
    template_id: null,
    title: '序号',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 48,
    is_hidden: 0,
    is_required: 1,
    name: 'name',
    no: 4,
    parent_no: 137,
    restriction_id: null,
    template_id: null,
    title: '名称',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 49,
    is_hidden: 0,
    is_required: 0,
    name: 'title',
    no: 5,
    parent_no: 137,
    restriction_id: null,
    template_id: null,
    title: '标题',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 50,
    is_hidden: 0,
    is_required: 1,
    name: 'is_hidden',
    no: 6,
    parent_no: 137,
    restriction_id: null,
    template_id: null,
    title: '隐藏',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 51,
    is_hidden: 0,
    is_required: 1,
    name: 'type_no',
    no: 7,
    parent_no: 137,
    restriction_id: null,
    template_id: null,
    title: '类型编号',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 52,
    is_hidden: 0,
    is_required: 0,
    name: 'is_required',
    no: 8,
    parent_no: 137,
    restriction_id: null,
    template_id: null,
    title: '必填',
    type_no: 4,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "id"}',
    id: 53,
    is_hidden: 0,
    is_required: 0,
    name: 'restriction_id',
    no: 9,
    parent_no: 137,
    restriction_id: null,
    template_id: null,
    title: '限制id',
    type_no: 68,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "symbol"}',
    id: 54,
    is_hidden: 0,
    is_required: 0,
    name: 'unit_id',
    no: 10,
    parent_no: 137,
    restriction_id: null,
    template_id: null,
    title: '单位id',
    type_no: 70,
    unit_id: null
  },
  {
    description: null,
    id: 55,
    is_hidden: 1,
    is_required: 0,
    name: 'description',
    no: 11,
    parent_no: 137,
    restriction_id: null,
    template_id: null,
    title: '备注',
    type_no: 2,
    unit_id: null
  },
  {
    description: '主键',
    id: 56,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 138,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "name"}',
    id: 57,
    is_hidden: 1,
    is_required: 1,
    name: 'template_id',
    no: 1,
    parent_no: 138,
    restriction_id: null,
    template_id: null,
    title: '模板id',
    type_no: 66,
    unit_id: null
  },
  {
    description: null,
    id: 58,
    is_hidden: 1,
    is_required: 1,
    name: 'parent_no',
    no: 2,
    parent_no: 138,
    restriction_id: null,
    template_id: null,
    title: '父编号',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 59,
    is_hidden: 0,
    is_required: 1,
    name: 'element_type_no',
    no: 3,
    parent_no: 138,
    restriction_id: null,
    template_id: null,
    title: '元素类型编号',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 60,
    is_hidden: 0,
    is_required: 0,
    name: 'tree_show_column_no',
    no: 4,
    parent_no: 138,
    restriction_id: null,
    template_id: null,
    title: '树显示列序号',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 61,
    is_hidden: 0,
    is_required: 0,
    name: 'min',
    no: 5,
    parent_no: 138,
    restriction_id: null,
    template_id: null,
    title: '最小行数',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 62,
    is_hidden: 0,
    is_required: 0,
    name: 'max',
    no: 6,
    parent_no: 138,
    restriction_id: null,
    template_id: null,
    title: '最大行数',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 63,
    is_hidden: 0,
    is_required: 0,
    name: 'group_by',
    no: 7,
    parent_no: 138,
    restriction_id: null,
    template_id: null,
    title: '分组列',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 64,
    is_hidden: 1,
    is_required: 0,
    name: 'description',
    no: 8,
    parent_no: 138,
    restriction_id: null,
    template_id: null,
    title: '备注',
    type_no: 2,
    unit_id: null
  },
  {
    description: '主键',
    id: 65,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 139,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 66,
    is_hidden: 1,
    is_required: 0,
    name: 'object_type',
    no: 1,
    parent_no: 139,
    restriction_id: null,
    template_id: null,
    title: '适用目标',
    type_no: 261,
    unit_id: null
  },
  {
    description: '批次或单机的id',
    id: 67,
    is_hidden: 1,
    is_required: 0,
    name: 'object_id',
    no: 2,
    parent_no: 139,
    restriction_id: null,
    template_id: null,
    title: '目标id',
    type_no: 4,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "id"}',
    id: 68,
    is_hidden: 1,
    is_required: 0,
    name: 'previous_version_id',
    no: 3,
    parent_no: 139,
    restriction_id: null,
    template_id: null,
    title: '前一版本id',
    type_no: 75,
    unit_id: null
  },
  {
    description: '主键',
    id: 69,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 140,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "name"}',
    id: 70,
    is_hidden: 71,
    is_required: 0,
    name: 'info_config_id',
    no: 1,
    parent_no: 140,
    restriction_id: null,
    template_id: null,
    title: '信息配置id',
    type_no: 1,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "id"}',
    id: 71,
    is_hidden: 0,
    is_required: 0,
    name: 'baseline_id',
    no: 2,
    parent_no: 140,
    restriction_id: null,
    template_id: null,
    title: '基线id',
    type_no: 75,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "id"}',
    id: 72,
    is_hidden: 0,
    is_required: 0,
    name: 'parent_id',
    no: 3,
    parent_no: 140,
    restriction_id: null,
    template_id: null,
    title: '父id',
    type_no: 76,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "id"}',
    id: 73,
    is_hidden: 0,
    is_required: 0,
    name: 'ref_id',
    no: 4,
    parent_no: 140,
    restriction_id: null,
    template_id: null,
    title: '引用id',
    type_no: 76,
    unit_id: null
  },
  {
    description: null,
    id: 74,
    is_hidden: 0,
    is_required: 0,
    name: 'serial_no',
    no: 5,
    parent_no: 140,
    restriction_id: null,
    template_id: null,
    title: '序号',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 75,
    is_hidden: 0,
    is_required: 0,
    name: 'data',
    no: 6,
    parent_no: 140,
    restriction_id: null,
    template_id: null,
    title: '数据',
    type_no: 2,
    unit_id: null
  },
  {
    description: '主键',
    id: 76,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 141,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 77,
    is_hidden: 0,
    is_required: 0,
    name: 'type',
    no: 1,
    parent_no: 141,
    restriction_id: null,
    template_id: null,
    title: 'mime-type',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 78,
    is_hidden: 0,
    is_required: 0,
    name: 'name',
    no: 2,
    parent_no: 141,
    restriction_id: null,
    template_id: null,
    title: '名称',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 79,
    is_hidden: 0,
    is_required: 0,
    name: 'ext',
    no: 3,
    parent_no: 141,
    restriction_id: null,
    template_id: null,
    title: '扩展名',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 80,
    is_hidden: 0,
    is_required: 0,
    name: 'content',
    no: 4,
    parent_no: 141,
    restriction_id: null,
    template_id: null,
    title: '内容',
    type_no: 8,
    unit_id: null
  },
  {
    description: null,
    id: 81,
    is_hidden: 0,
    is_required: 0,
    name: 'size',
    no: 5,
    parent_no: 141,
    restriction_id: null,
    template_id: null,
    title: '文件大小',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 82,
    is_hidden: 0,
    is_required: 0,
    name: 'digest',
    no: 6,
    parent_no: 141,
    restriction_id: null,
    template_id: null,
    title: '内容摘要',
    type_no: 1,
    unit_id: null
  },
  {
    description: '主键',
    id: 83,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 142,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 84,
    is_hidden: 0,
    is_required: 0,
    name: 'no',
    no: 1,
    parent_no: 142,
    restriction_id: null,
    template_id: null,
    title: '编号',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 85,
    is_hidden: 0,
    is_required: 0,
    name: 'name',
    no: 2,
    parent_no: 142,
    restriction_id: null,
    template_id: null,
    title: '名称',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 86,
    is_hidden: 1,
    is_required: 0,
    name: 'type',
    no: 3,
    parent_no: 142,
    restriction_id: null,
    template_id: null,
    title: '类型',
    type_no: 262,
    unit_id: null
  },
  {
    description: null,
    id: 87,
    is_hidden: 1,
    is_required: 0,
    name: 'address',
    no: 4,
    parent_no: 142,
    restriction_id: null,
    template_id: null,
    title: '地址',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 88,
    is_hidden: 1,
    is_required: 0,
    name: 'telephone',
    no: 5,
    parent_no: 142,
    restriction_id: null,
    template_id: null,
    title: '电话',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 89,
    is_hidden: 0,
    is_required: 0,
    name: 'contact',
    no: 6,
    parent_no: 142,
    restriction_id: null,
    template_id: null,
    title: '联系人',
    type_no: 1,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "name", "filter": "\\"organization_id\\" = id"}',
    id: 90,
    is_hidden: 1,
    is_required: 0,
    name: 'manager_id',
    no: 7,
    parent_no: 142,
    restriction_id: null,
    template_id: null,
    title: '管理员id',
    type_no: 79,
    unit_id: null
  },
  {
    description: null,
    id: 91,
    is_hidden: 1,
    is_required: 0,
    name: 'description',
    no: 8,
    parent_no: 142,
    restriction_id: null,
    template_id: null,
    title: '备注',
    type_no: 2,
    unit_id: null
  },
  {
    description: '主键',
    id: 92,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 143,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 93,
    is_hidden: 1,
    is_required: 1,
    name: 'login_name',
    no: 1,
    parent_no: 143,
    restriction_id: null,
    template_id: null,
    title: '登录名',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 94,
    is_hidden: 0,
    is_required: 0,
    name: 'name',
    no: 2,
    parent_no: 143,
    restriction_id: null,
    template_id: null,
    title: '姓名',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 95,
    is_hidden: 1,
    is_required: 0,
    name: 'sex',
    no: 3,
    parent_no: 143,
    restriction_id: null,
    template_id: null,
    title: '性别',
    type_no: 263,
    unit_id: null
  },
  {
    description: null,
    id: 96,
    is_hidden: 1,
    is_required: 0,
    name: 'birthday',
    no: 4,
    parent_no: 143,
    restriction_id: null,
    template_id: null,
    title: '生日',
    type_no: 6,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "name"}',
    id: 97,
    is_hidden: 1,
    is_required: 0,
    name: 'organization_id',
    no: 5,
    parent_no: 143,
    restriction_id: null,
    template_id: null,
    title: '组织id',
    type_no: 78,
    unit_id: null
  },
  {
    description: null,
    id: 98,
    is_hidden: 1,
    is_required: 0,
    name: 'address',
    no: 6,
    parent_no: 143,
    restriction_id: null,
    template_id: null,
    title: '住址',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 99,
    is_hidden: 1,
    is_required: 0,
    name: 'telephone',
    no: 7,
    parent_no: 143,
    restriction_id: null,
    template_id: null,
    title: '电话',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 100,
    is_hidden: 1,
    is_required: 0,
    name: 'email',
    no: 8,
    parent_no: 143,
    restriction_id: null,
    template_id: null,
    title: '电子邮箱',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 101,
    is_hidden: 1,
    is_required: 0,
    name: 'website',
    no: 9,
    parent_no: 143,
    restriction_id: null,
    template_id: null,
    title: '网站',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 102,
    is_hidden: 1,
    is_required: 1,
    name: 'password',
    no: 10,
    parent_no: 143,
    restriction_id: null,
    template_id: null,
    title: '密码',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 103,
    is_hidden: 0,
    is_required: 0,
    name: 'role',
    no: 11,
    parent_no: 143,
    restriction_id: null,
    template_id: null,
    title: '角色',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 104,
    is_hidden: 0,
    is_required: 0,
    name: 'description',
    no: 12,
    parent_no: 143,
    restriction_id: null,
    template_id: null,
    title: '备注',
    type_no: 2,
    unit_id: null
  },
  {
    description: '主键',
    id: 105,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 144,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 106,
    is_hidden: 1,
    is_required: 0,
    name: 'time',
    no: 1,
    parent_no: 144,
    restriction_id: null,
    template_id: null,
    title: '时间',
    type_no: 6,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "name"}',
    id: 107,
    is_hidden: 0,
    is_required: 0,
    name: 'operator_id',
    no: 2,
    parent_no: 144,
    restriction_id: null,
    template_id: null,
    title: '操作人id',
    type_no: 79,
    unit_id: null
  },
  {
    description: null,
    id: 108,
    is_hidden: 0,
    is_required: 0,
    name: 'data_type',
    no: 3,
    parent_no: 144,
    restriction_id: null,
    template_id: null,
    title: '数据类型',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 109,
    is_hidden: 0,
    is_required: 0,
    name: 'data_item',
    no: 4,
    parent_no: 144,
    restriction_id: null,
    template_id: null,
    title: '数据项',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 110,
    is_hidden: 1,
    is_required: 0,
    name: 'action',
    no: 5,
    parent_no: 144,
    restriction_id: null,
    template_id: null,
    title: '动作',
    type_no: 2,
    unit_id: null
  },
  {
    description: null,
    id: 111,
    is_hidden: 1,
    is_required: 0,
    name: 'abstract',
    no: 6,
    parent_no: 144,
    restriction_id: null,
    template_id: null,
    title: '摘要',
    type_no: 2,
    unit_id: null
  },
  {
    description: null,
    id: 112,
    is_hidden: 0,
    is_required: 0,
    name: 'description',
    no: 7,
    parent_no: 144,
    restriction_id: null,
    template_id: null,
    title: '备注',
    type_no: 2,
    unit_id: null
  },
  {
    description: '主键',
    id: 113,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 145,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 114,
    is_hidden: 1,
    is_required: 0,
    name: 'template_id',
    no: 1,
    parent_no: 145,
    restriction_id: null,
    template_id: null,
    title: '模板id',
    type_no: 66,
    unit_id: null
  },
  {
    description: null,
    id: 115,
    is_hidden: 0,
    is_required: 0,
    name: 'type',
    no: 2,
    parent_no: 145,
    restriction_id: null,
    template_id: null,
    title: '类型',
    type_no: 264,
    unit_id: null
  },
  {
    description: null,
    id: 116,
    is_hidden: 0,
    is_required: 0,
    name: 'name',
    no: 3,
    parent_no: 145,
    restriction_id: null,
    template_id: null,
    title: '名称',
    type_no: 1,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "name", "filter": "\\"type\\" = 1"}',
    id: 117,
    is_hidden: 0,
    is_required: 0,
    name: 'factory_id',
    no: 4,
    parent_no: 145,
    restriction_id: null,
    template_id: null,
    title: '工厂id',
    type_no: 78,
    unit_id: null
  },
  {
    description: null,
    id: 118,
    is_hidden: 1,
    is_required: 0,
    name: 'state',
    no: 5,
    parent_no: 145,
    restriction_id: null,
    template_id: null,
    title: '状态',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 119,
    is_hidden: 0,
    is_required: 0,
    name: 'description',
    no: 6,
    parent_no: 145,
    restriction_id: null,
    template_id: null,
    title: '备注',
    type_no: 2,
    unit_id: null
  },
  {
    description: '主键',
    id: 120,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 146,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "name"}',
    id: 121,
    is_hidden: 1,
    is_required: 0,
    name: 'model_id',
    no: 1,
    parent_no: 146,
    restriction_id: null,
    template_id: null,
    title: '机型id',
    type_no: 81,
    unit_id: null
  },
  {
    description: null,
    id: 122,
    is_hidden: 0,
    is_required: 0,
    name: 'no',
    no: 2,
    parent_no: 146,
    restriction_id: null,
    template_id: null,
    title: '编号',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 123,
    is_hidden: 1,
    is_required: 0,
    name: 'state',
    no: 3,
    parent_no: 146,
    restriction_id: null,
    template_id: null,
    title: '状态',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 124,
    is_hidden: 0,
    is_required: 0,
    name: 'description',
    no: 4,
    parent_no: 146,
    restriction_id: null,
    template_id: null,
    title: '备注',
    type_no: 2,
    unit_id: null
  },
  {
    description: '主键',
    id: 125,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 147,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "no"}',
    id: 126,
    is_hidden: 1,
    is_required: 0,
    name: 'state_id',
    no: 1,
    parent_no: 147,
    restriction_id: null,
    template_id: null,
    title: '状态id',
    type_no: 82,
    unit_id: null
  },
  {
    description: null,
    id: 127,
    is_hidden: 0,
    is_required: 0,
    name: 'no',
    no: 2,
    parent_no: 147,
    restriction_id: null,
    template_id: null,
    title: '编号',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 128,
    is_hidden: 0,
    is_required: 0,
    name: 'air_no_range',
    no: 3,
    parent_no: 147,
    restriction_id: null,
    template_id: null,
    title: '飞机编号范围',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 129,
    is_hidden: 0,
    is_required: 0,
    name: 'engine_no_range',
    no: 4,
    parent_no: 147,
    restriction_id: null,
    template_id: null,
    title: '发动机编号范围',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 130,
    is_hidden: 1,
    is_required: 0,
    name: 'state',
    no: 5,
    parent_no: 147,
    restriction_id: null,
    template_id: null,
    title: '状态',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 131,
    is_hidden: 0,
    is_required: 0,
    name: 'description',
    no: 6,
    parent_no: 147,
    restriction_id: null,
    template_id: null,
    title: '备注',
    type_no: 2,
    unit_id: null
  },
  {
    description: '主键',
    id: 132,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 148,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "no"}',
    id: 133,
    is_hidden: 1,
    is_required: 0,
    name: 'batch_id',
    no: 1,
    parent_no: 149,
    restriction_id: null,
    template_id: null,
    title: '批次id',
    type_no: 83,
    unit_id: null
  },
  {
    description: null,
    id: 134,
    is_hidden: 0,
    is_required: 0,
    name: 'tag',
    no: 2,
    parent_no: 148,
    restriction_id: null,
    template_id: null,
    title: 'tag',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 135,
    is_hidden: 0,
    is_required: 0,
    name: 'no',
    no: 3,
    parent_no: 148,
    restriction_id: null,
    template_id: null,
    title: '编号',
    type_no: 1,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "name", "filter": "\\"type\\" = 1"}',
    id: 136,
    is_hidden: 0,
    is_required: 0,
    name: 'factory_id',
    no: 4,
    parent_no: 148,
    restriction_id: null,
    template_id: null,
    title: '承制单位id',
    type_no: 81,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "name", "filter": "\\"type\\" = 2"}',
    id: 137,
    is_hidden: 1,
    is_required: 0,
    name: 'user_id',
    no: 5,
    parent_no: 148,
    restriction_id: null,
    template_id: null,
    title: '接装单位id',
    type_no: 81,
    unit_id: null
  },
  {
    description: null,
    id: 138,
    is_hidden: 0,
    is_required: 0,
    name: 'offer_time',
    no: 6,
    parent_no: 148,
    restriction_id: null,
    template_id: null,
    title: '移交时间',
    type_no: 6,
    unit_id: null
  },
  {
    description: null,
    id: 139,
    is_hidden: 0,
    is_required: 0,
    name: 'offer_no',
    no: 7,
    parent_no: 148,
    restriction_id: null,
    template_id: null,
    title: '移交编号',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 140,
    is_hidden: 0,
    is_required: 0,
    name: 'lifespan',
    no: 8,
    parent_no: 148,
    restriction_id: null,
    template_id: null,
    title: '使用寿命',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 141,
    is_hidden: 0,
    is_required: 0,
    name: 'residue_span',
    no: 9,
    parent_no: 148,
    restriction_id: null,
    template_id: null,
    title: '剩余寿命',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 142,
    is_hidden: 0,
    is_required: 0,
    name: 'engine_model',
    no: 10,
    parent_no: 148,
    restriction_id: null,
    template_id: null,
    title: '引擎型号',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 143,
    is_hidden: 0,
    is_required: 0,
    name: 'engine_no',
    no: 11,
    parent_no: 148,
    restriction_id: null,
    template_id: null,
    title: '引擎编号',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 144,
    is_hidden: 0,
    is_required: 0,
    name: 'engine_lifespan',
    no: 12,
    parent_no: 148,
    restriction_id: null,
    template_id: null,
    title: '引擎使用寿命',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 145,
    is_hidden: 0,
    is_required: 0,
    name: 'engine_residue_span',
    no: 13,
    parent_no: 148,
    restriction_id: null,
    template_id: null,
    title: '引擎剩余寿命',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 146,
    is_hidden: 1,
    is_required: 0,
    name: 'state',
    no: 14,
    parent_no: 148,
    restriction_id: null,
    template_id: null,
    title: '状态',
    type_no: 4,
    unit_id: null
  },
  {
    description: null,
    id: 147,
    is_hidden: 0,
    is_required: 0,
    name: 'description',
    no: 15,
    parent_no: 148,
    restriction_id: null,
    template_id: null,
    title: '备注',
    type_no: 2,
    unit_id: null
  },
  {
    description: '主键',
    id: 148,
    is_hidden: 1,
    is_required: 1,
    name: 'id',
    no: 0,
    parent_no: 149,
    restriction_id: null,
    template_id: null,
    title: 'id',
    type_no: 4,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "name"}',
    id: 149,
    is_hidden: 1,
    is_required: 0,
    name: 'baseline_id',
    no: 1,
    parent_no: 149,
    restriction_id: null,
    template_id: null,
    title: '基线id',
    type_no: 75,
    unit_id: null
  },
  {
    description: null,
    id: 150,
    is_hidden: 0,
    is_required: 0,
    name: 'name',
    no: 2,
    parent_no: 149,
    restriction_id: null,
    template_id: null,
    title: '名称',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 151,
    is_hidden: 0,
    is_required: 0,
    name: 'no',
    no: 3,
    parent_no: 149,
    restriction_id: null,
    template_id: null,
    title: '编号',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 152,
    is_hidden: 0,
    is_required: 0,
    name: 'object',
    no: 4,
    parent_no: 149,
    restriction_id: null,
    template_id: null,
    title: '对象',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 153,
    is_hidden: 1,
    is_required: 0,
    name: 'abstract',
    no: 5,
    parent_no: 149,
    restriction_id: null,
    template_id: null,
    title: '摘要说明',
    type_no: 2,
    unit_id: null
  },
  {
    description: null,
    id: 154,
    is_hidden: 0,
    is_required: 0,
    name: 'operator',
    no: 6,
    parent_no: 149,
    restriction_id: null,
    template_id: null,
    title: '经手人',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 155,
    is_hidden: 0,
    is_required: 0,
    name: 'time',
    no: 7,
    parent_no: 149,
    restriction_id: null,
    template_id: null,
    title: '时间',
    type_no: 1,
    unit_id: null
  },
  {
    description: '{"value": "id", "show": "name"}',
    id: 156,
    is_hidden: 0,
    is_required: 0,
    name: 'control_file_id',
    no: 8,
    parent_no: 149,
    restriction_id: null,
    template_id: null,
    title: '控制文件id',
    type_no: 1,
    unit_id: null
  },
  {
    description: null,
    id: 157,
    is_hidden: 0,
    is_required: 0,
    name: 'description',
    no: 15,
    parent_no: 149,
    restriction_id: null,
    template_id: null,
    title: '备注',
    type_no: 2,
    unit_id: null
  }
]
// id, template_id, parent_no, element_type_no, tree_show_column_no, min, max, group_by, description
const lists = [
  {
      "description": "主键",
      "id": 1,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 129,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 2,
      "is_hidden": 0,
      "is_required": 0,
      "name": "name",
      "no": 1,
      "parent_no": 129,
      "restriction_id": null,
      "template_id": null,
      "title": "名称",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 3,
      "is_hidden": 0,
      "is_required": 0,
      "name": "value",
      "no": 2,
      "parent_no": 129,
      "restriction_id": null,
      "template_id": null,
      "title": "值",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 4,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 130,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 5,
      "is_hidden": 0,
      "is_required": 1,
      "name": "name",
      "no": 1,
      "parent_no": 130,
      "restriction_id": null,
      "template_id": null,
      "title": "名称",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 6,
      "is_hidden": 1,
      "is_required": 1,
      "name": "state",
      "no": 2,
      "parent_no": 130,
      "restriction_id": null,
      "template_id": null,
      "title": "状态",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 7,
      "is_hidden": 0,
      "is_required": 0,
      "name": "description",
      "no": 3,
      "parent_no": 130,
      "restriction_id": null,
      "template_id": null,
      "title": "备注",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 8,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 131,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 9,
      "is_hidden": 0,
      "is_required": 1,
      "name": "no",
      "no": 1,
      "parent_no": 131,
      "restriction_id": null,
      "template_id": null,
      "title": "编号",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"name\"}",
      "id": 10,
      "is_hidden": 1,
      "is_required": 0,
      "name": "template_id",
      "no": 2,
      "parent_no": 131,
      "restriction_id": null,
      "template_id": null,
      "title": "模板id",
      "type_no": 66,
      "unit_id": null
  },
  {
      "description": null,
      "id": 11,
      "is_hidden": 0,
      "is_required": 1,
      "name": "name",
      "no": 3,
      "parent_no": 131,
      "restriction_id": null,
      "template_id": null,
      "title": "名称",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 12,
      "is_hidden": 0,
      "is_required": 1,
      "name": "type",
      "no": 4,
      "parent_no": 131,
      "restriction_id": null,
      "template_id": null,
      "title": "类型",
      "type_no": 257,
      "unit_id": null
  },
  {
      "description": null,
      "id": 13,
      "is_hidden": 1,
      "is_required": 0,
      "name": "description",
      "no": 5,
      "parent_no": 131,
      "restriction_id": null,
      "template_id": null,
      "title": "备注",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 14,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 132,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"name\"}",
      "id": 15,
      "is_hidden": 1,
      "is_required": 1,
      "name": "template_id",
      "no": 1,
      "parent_no": 132,
      "restriction_id": null,
      "template_id": null,
      "title": "模板id",
      "type_no": 66,
      "unit_id": null
  },
  {
      "description": null,
      "id": 16,
      "is_hidden": 0,
      "is_required": 0,
      "name": "length",
      "no": 2,
      "parent_no": 132,
      "restriction_id": null,
      "template_id": null,
      "title": "长度",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 17,
      "is_hidden": 0,
      "is_required": 0,
      "name": "format",
      "no": 3,
      "parent_no": 132,
      "restriction_id": null,
      "template_id": null,
      "title": "格式",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 18,
      "is_hidden": 0,
      "is_required": 0,
      "name": "precision",
      "no": 4,
      "parent_no": 132,
      "restriction_id": null,
      "template_id": null,
      "title": "精度",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 19,
      "is_hidden": 0,
      "is_required": 0,
      "name": "min",
      "no": 5,
      "parent_no": 132,
      "restriction_id": null,
      "template_id": null,
      "title": "最小值",
      "type_no": 5,
      "unit_id": null
  },
  {
      "description": null,
      "id": 20,
      "is_hidden": 1,
      "is_required": 0,
      "name": "max",
      "no": 6,
      "parent_no": 132,
      "restriction_id": null,
      "template_id": null,
      "title": "最大值",
      "type_no": 5,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 21,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 133,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 22,
      "is_hidden": 0,
      "is_required": 1,
      "name": "name",
      "no": 1,
      "parent_no": 133,
      "restriction_id": null,
      "template_id": null,
      "title": "名称",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 23,
      "is_hidden": 0,
      "is_required": 0,
      "name": "description",
      "no": 2,
      "parent_no": 133,
      "restriction_id": null,
      "template_id": null,
      "title": "备注",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 24,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 134,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"name\"}",
      "id": 25,
      "is_hidden": 1,
      "is_required": 0,
      "name": "type_id",
      "no": 1,
      "parent_no": 134,
      "restriction_id": null,
      "template_id": null,
      "title": "范畴id",
      "type_no": 69,
      "unit_id": null
  },
  {
      "description": null,
      "id": 26,
      "is_hidden": 0,
      "is_required": 1,
      "name": "name",
      "no": 2,
      "parent_no": 134,
      "restriction_id": null,
      "template_id": null,
      "title": "名称",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 27,
      "is_hidden": 0,
      "is_required": 0,
      "name": "symbol",
      "no": 3,
      "parent_no": 134,
      "restriction_id": null,
      "template_id": null,
      "title": "符号",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 28,
      "is_hidden": 0,
      "is_required": 0,
      "name": "description",
      "no": 4,
      "parent_no": 134,
      "restriction_id": null,
      "template_id": null,
      "title": "备注",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 29,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 135,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"name\"}",
      "id": 30,
      "is_hidden": 1,
      "is_required": 1,
      "name": "template_id",
      "no": 1,
      "parent_no": 135,
      "restriction_id": null,
      "template_id": null,
      "title": "模板id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"name\"}",
      "id": 31,
      "is_hidden": 0,
      "is_required": 0,
      "name": "parent_id",
      "no": 2,
      "parent_no": 135,
      "restriction_id": null,
      "template_id": null,
      "title": "父id",
      "type_no": 71,
      "unit_id": null
  },
  {
      "description": null,
      "id": 32,
      "is_hidden": 0,
      "is_required": 1,
      "name": "type",
      "no": 3,
      "parent_no": 135,
      "restriction_id": null,
      "template_id": null,
      "title": "类型",
      "type_no": 258,
      "unit_id": null
  },
  {
      "description": null,
      "id": 33,
      "is_hidden": 0,
      "is_required": 1,
      "name": "no",
      "no": 4,
      "parent_no": 135,
      "restriction_id": null,
      "template_id": null,
      "title": "序号",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 34,
      "is_hidden": 0,
      "is_required": 1,
      "name": "name",
      "no": 5,
      "parent_no": 135,
      "restriction_id": null,
      "template_id": null,
      "title": "名称",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 35,
      "is_hidden": 0,
      "is_required": 1,
      "name": "type_no",
      "no": 6,
      "parent_no": 135,
      "restriction_id": null,
      "template_id": null,
      "title": "类型编号",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"name\"}",
      "id": 36,
      "is_hidden": 0,
      "is_required": 0,
      "name": "reference_to",
      "no": 7,
      "parent_no": 135,
      "restriction_id": null,
      "template_id": null,
      "title": "引用到",
      "type_no": 71,
      "unit_id": null
  },
  {
      "description": null,
      "id": 37,
      "is_hidden": 0,
      "is_required": 0,
      "name": "is_required",
      "no": 8,
      "parent_no": 135,
      "restriction_id": null,
      "template_id": null,
      "title": "必填",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"id\"}",
      "id": 38,
      "is_hidden": 0,
      "is_required": 0,
      "name": "restriction_id",
      "no": 9,
      "parent_no": 135,
      "restriction_id": null,
      "template_id": null,
      "title": "限制id",
      "type_no": 68,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"symbol\"}",
      "id": 39,
      "is_hidden": 0,
      "is_required": 0,
      "name": "unit_id",
      "no": 10,
      "parent_no": 135,
      "restriction_id": null,
      "template_id": null,
      "title": "单位id",
      "type_no": 70,
      "unit_id": null
  },
  {
      "description": null,
      "id": 40,
      "is_hidden": 1,
      "is_required": 0,
      "name": "description",
      "no": 11,
      "parent_no": 135,
      "restriction_id": null,
      "template_id": null,
      "title": "备注",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 41,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 136,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"name\"}",
      "id": 42,
      "is_hidden": 1,
      "is_required": 1,
      "name": "template_id",
      "no": 1,
      "parent_no": 136,
      "restriction_id": null,
      "template_id": null,
      "title": "模板id",
      "type_no": 66,
      "unit_id": null
  },
  {
      "description": null,
      "id": 43,
      "is_hidden": 1,
      "is_required": 1,
      "name": "parent_no",
      "no": 2,
      "parent_no": 136,
      "restriction_id": null,
      "template_id": null,
      "title": "父编号",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 44,
      "is_hidden": 0,
      "is_required": 1,
      "name": "no",
      "no": 3,
      "parent_no": 136,
      "restriction_id": null,
      "template_id": null,
      "title": "序号",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 45,
      "is_hidden": 0,
      "is_required": 1,
      "name": "value",
      "no": 4,
      "parent_no": 136,
      "restriction_id": null,
      "template_id": null,
      "title": "值",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 46,
      "is_hidden": 1,
      "is_required": 0,
      "name": "description",
      "no": 5,
      "parent_no": 136,
      "restriction_id": null,
      "template_id": null,
      "title": "备注",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 47,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 137,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"name\"}",
      "id": 48,
      "is_hidden": 1,
      "is_required": 1,
      "name": "template_id",
      "no": 1,
      "parent_no": 137,
      "restriction_id": null,
      "template_id": null,
      "title": "模板id",
      "type_no": 66,
      "unit_id": null
  },
  {
      "description": null,
      "id": 49,
      "is_hidden": 1,
      "is_required": 1,
      "name": "parent_no",
      "no": 2,
      "parent_no": 137,
      "restriction_id": null,
      "template_id": null,
      "title": "父编号",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 50,
      "is_hidden": 1,
      "is_required": 1,
      "name": "no",
      "no": 3,
      "parent_no": 137,
      "restriction_id": null,
      "template_id": null,
      "title": "序号",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 51,
      "is_hidden": 0,
      "is_required": 1,
      "name": "name",
      "no": 4,
      "parent_no": 137,
      "restriction_id": null,
      "template_id": null,
      "title": "名称",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 52,
      "is_hidden": 0,
      "is_required": 0,
      "name": "title",
      "no": 5,
      "parent_no": 137,
      "restriction_id": null,
      "template_id": null,
      "title": "标题",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 53,
      "is_hidden": 0,
      "is_required": 1,
      "name": "is_hidden",
      "no": 6,
      "parent_no": 137,
      "restriction_id": null,
      "template_id": null,
      "title": "隐藏",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 54,
      "is_hidden": 0,
      "is_required": 1,
      "name": "type_no",
      "no": 7,
      "parent_no": 137,
      "restriction_id": null,
      "template_id": null,
      "title": "类型编号",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 55,
      "is_hidden": 0,
      "is_required": 0,
      "name": "is_required",
      "no": 8,
      "parent_no": 137,
      "restriction_id": null,
      "template_id": null,
      "title": "必填",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"id\"}",
      "id": 56,
      "is_hidden": 0,
      "is_required": 0,
      "name": "restriction_id",
      "no": 9,
      "parent_no": 137,
      "restriction_id": null,
      "template_id": null,
      "title": "限制id",
      "type_no": 68,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"symbol\"}",
      "id": 57,
      "is_hidden": 0,
      "is_required": 0,
      "name": "unit_id",
      "no": 10,
      "parent_no": 137,
      "restriction_id": null,
      "template_id": null,
      "title": "单位id",
      "type_no": 70,
      "unit_id": null
  },
  {
      "description": null,
      "id": 58,
      "is_hidden": 1,
      "is_required": 0,
      "name": "description",
      "no": 11,
      "parent_no": 137,
      "restriction_id": null,
      "template_id": null,
      "title": "备注",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 59,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 138,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"name\"}",
      "id": 60,
      "is_hidden": 1,
      "is_required": 1,
      "name": "template_id",
      "no": 1,
      "parent_no": 138,
      "restriction_id": null,
      "template_id": null,
      "title": "模板id",
      "type_no": 66,
      "unit_id": null
  },
  {
      "description": null,
      "id": 61,
      "is_hidden": 1,
      "is_required": 1,
      "name": "parent_no",
      "no": 2,
      "parent_no": 138,
      "restriction_id": null,
      "template_id": null,
      "title": "父编号",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 62,
      "is_hidden": 0,
      "is_required": 1,
      "name": "element_type_no",
      "no": 3,
      "parent_no": 138,
      "restriction_id": null,
      "template_id": null,
      "title": "元素类型编号",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 63,
      "is_hidden": 0,
      "is_required": 0,
      "name": "tree_show_column_no",
      "no": 4,
      "parent_no": 138,
      "restriction_id": null,
      "template_id": null,
      "title": "树显示列序号",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 64,
      "is_hidden": 0,
      "is_required": 0,
      "name": "min",
      "no": 5,
      "parent_no": 138,
      "restriction_id": null,
      "template_id": null,
      "title": "最小行数",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 65,
      "is_hidden": 0,
      "is_required": 0,
      "name": "max",
      "no": 6,
      "parent_no": 138,
      "restriction_id": null,
      "template_id": null,
      "title": "最大行数",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 66,
      "is_hidden": 0,
      "is_required": 0,
      "name": "group_by",
      "no": 7,
      "parent_no": 138,
      "restriction_id": null,
      "template_id": null,
      "title": "分组列",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 67,
      "is_hidden": 1,
      "is_required": 0,
      "name": "description",
      "no": 8,
      "parent_no": 138,
      "restriction_id": null,
      "template_id": null,
      "title": "备注",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 68,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 139,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 69,
      "is_hidden": 1,
      "is_required": 0,
      "name": "object_type",
      "no": 1,
      "parent_no": 139,
      "restriction_id": null,
      "template_id": null,
      "title": "适用目标",
      "type_no": 261,
      "unit_id": null
  },
  {
      "description": "批次或单机的id",
      "id": 70,
      "is_hidden": 1,
      "is_required": 0,
      "name": "object_id",
      "no": 2,
      "parent_no": 139,
      "restriction_id": null,
      "template_id": null,
      "title": "目标id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"id\"}",
      "id": 71,
      "is_hidden": 1,
      "is_required": 0,
      "name": "previous_version_id",
      "no": 3,
      "parent_no": 139,
      "restriction_id": null,
      "template_id": null,
      "title": "前一版本id",
      "type_no": 75,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 72,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 140,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"name\"}",
      "id": 73,
      "is_hidden": 71,
      "is_required": 0,
      "name": "info_config_id",
      "no": 1,
      "parent_no": 140,
      "restriction_id": null,
      "template_id": null,
      "title": "信息配置id",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"id\"}",
      "id": 74,
      "is_hidden": 0,
      "is_required": 0,
      "name": "baseline_id",
      "no": 2,
      "parent_no": 140,
      "restriction_id": null,
      "template_id": null,
      "title": "基线id",
      "type_no": 75,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"id\"}",
      "id": 75,
      "is_hidden": 0,
      "is_required": 0,
      "name": "parent_id",
      "no": 3,
      "parent_no": 140,
      "restriction_id": null,
      "template_id": null,
      "title": "父id",
      "type_no": 76,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"id\"}",
      "id": 76,
      "is_hidden": 0,
      "is_required": 0,
      "name": "ref_id",
      "no": 4,
      "parent_no": 140,
      "restriction_id": null,
      "template_id": null,
      "title": "引用id",
      "type_no": 76,
      "unit_id": null
  },
  {
      "description": null,
      "id": 77,
      "is_hidden": 0,
      "is_required": 0,
      "name": "serial_no",
      "no": 5,
      "parent_no": 140,
      "restriction_id": null,
      "template_id": null,
      "title": "序号",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 78,
      "is_hidden": 0,
      "is_required": 0,
      "name": "data",
      "no": 6,
      "parent_no": 140,
      "restriction_id": null,
      "template_id": null,
      "title": "数据",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 79,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 141,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 80,
      "is_hidden": 0,
      "is_required": 0,
      "name": "type",
      "no": 1,
      "parent_no": 141,
      "restriction_id": null,
      "template_id": null,
      "title": "mime-type",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 81,
      "is_hidden": 0,
      "is_required": 0,
      "name": "name",
      "no": 2,
      "parent_no": 141,
      "restriction_id": null,
      "template_id": null,
      "title": "名称",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 82,
      "is_hidden": 0,
      "is_required": 0,
      "name": "ext",
      "no": 3,
      "parent_no": 141,
      "restriction_id": null,
      "template_id": null,
      "title": "扩展名",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 83,
      "is_hidden": 0,
      "is_required": 0,
      "name": "content",
      "no": 4,
      "parent_no": 141,
      "restriction_id": null,
      "template_id": null,
      "title": "内容",
      "type_no": 8,
      "unit_id": null
  },
  {
      "description": null,
      "id": 84,
      "is_hidden": 0,
      "is_required": 0,
      "name": "size",
      "no": 5,
      "parent_no": 141,
      "restriction_id": null,
      "template_id": null,
      "title": "文件大小",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 85,
      "is_hidden": 0,
      "is_required": 0,
      "name": "digest",
      "no": 6,
      "parent_no": 141,
      "restriction_id": null,
      "template_id": null,
      "title": "内容摘要",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 86,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 142,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 87,
      "is_hidden": 0,
      "is_required": 0,
      "name": "no",
      "no": 1,
      "parent_no": 142,
      "restriction_id": null,
      "template_id": null,
      "title": "编号",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 88,
      "is_hidden": 0,
      "is_required": 0,
      "name": "name",
      "no": 2,
      "parent_no": 142,
      "restriction_id": null,
      "template_id": null,
      "title": "名称",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 89,
      "is_hidden": 1,
      "is_required": 0,
      "name": "type",
      "no": 3,
      "parent_no": 142,
      "restriction_id": null,
      "template_id": null,
      "title": "类型",
      "type_no": 262,
      "unit_id": null
  },
  {
      "description": null,
      "id": 90,
      "is_hidden": 1,
      "is_required": 0,
      "name": "address",
      "no": 4,
      "parent_no": 142,
      "restriction_id": null,
      "template_id": null,
      "title": "地址",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 91,
      "is_hidden": 1,
      "is_required": 0,
      "name": "telephone",
      "no": 5,
      "parent_no": 142,
      "restriction_id": null,
      "template_id": null,
      "title": "电话",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 92,
      "is_hidden": 0,
      "is_required": 0,
      "name": "contact",
      "no": 6,
      "parent_no": 142,
      "restriction_id": null,
      "template_id": null,
      "title": "联系人",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"name\", \"filter\": \"\\\"organization_id\\\" = id\"}",
      "id": 93,
      "is_hidden": 1,
      "is_required": 0,
      "name": "manager_id",
      "no": 7,
      "parent_no": 142,
      "restriction_id": null,
      "template_id": null,
      "title": "管理员id",
      "type_no": 79,
      "unit_id": null
  },
  {
      "description": null,
      "id": 94,
      "is_hidden": 1,
      "is_required": 0,
      "name": "description",
      "no": 8,
      "parent_no": 142,
      "restriction_id": null,
      "template_id": null,
      "title": "备注",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 95,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 143,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 96,
      "is_hidden": 1,
      "is_required": 1,
      "name": "login_name",
      "no": 1,
      "parent_no": 143,
      "restriction_id": null,
      "template_id": null,
      "title": "登录名",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 97,
      "is_hidden": 0,
      "is_required": 0,
      "name": "name",
      "no": 2,
      "parent_no": 143,
      "restriction_id": null,
      "template_id": null,
      "title": "姓名",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 98,
      "is_hidden": 1,
      "is_required": 0,
      "name": "sex",
      "no": 3,
      "parent_no": 143,
      "restriction_id": null,
      "template_id": null,
      "title": "性别",
      "type_no": 263,
      "unit_id": null
  },
  {
      "description": null,
      "id": 99,
      "is_hidden": 1,
      "is_required": 0,
      "name": "birthday",
      "no": 4,
      "parent_no": 143,
      "restriction_id": null,
      "template_id": null,
      "title": "生日",
      "type_no": 6,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"name\"}",
      "id": 100,
      "is_hidden": 1,
      "is_required": 0,
      "name": "organization_id",
      "no": 5,
      "parent_no": 143,
      "restriction_id": null,
      "template_id": null,
      "title": "组织id",
      "type_no": 78,
      "unit_id": null
  },
  {
      "description": null,
      "id": 101,
      "is_hidden": 1,
      "is_required": 0,
      "name": "address",
      "no": 6,
      "parent_no": 143,
      "restriction_id": null,
      "template_id": null,
      "title": "住址",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 102,
      "is_hidden": 1,
      "is_required": 0,
      "name": "telephone",
      "no": 7,
      "parent_no": 143,
      "restriction_id": null,
      "template_id": null,
      "title": "电话",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 103,
      "is_hidden": 1,
      "is_required": 0,
      "name": "email",
      "no": 8,
      "parent_no": 143,
      "restriction_id": null,
      "template_id": null,
      "title": "电子邮箱",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 104,
      "is_hidden": 1,
      "is_required": 0,
      "name": "website",
      "no": 9,
      "parent_no": 143,
      "restriction_id": null,
      "template_id": null,
      "title": "网站",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 105,
      "is_hidden": 1,
      "is_required": 1,
      "name": "password",
      "no": 10,
      "parent_no": 143,
      "restriction_id": null,
      "template_id": null,
      "title": "密码",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 106,
      "is_hidden": 0,
      "is_required": 0,
      "name": "role",
      "no": 11,
      "parent_no": 143,
      "restriction_id": null,
      "template_id": null,
      "title": "角色",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 107,
      "is_hidden": 0,
      "is_required": 0,
      "name": "description",
      "no": 12,
      "parent_no": 143,
      "restriction_id": null,
      "template_id": null,
      "title": "备注",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 108,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 144,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 109,
      "is_hidden": 1,
      "is_required": 0,
      "name": "time",
      "no": 1,
      "parent_no": 144,
      "restriction_id": null,
      "template_id": null,
      "title": "时间",
      "type_no": 6,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"name\"}",
      "id": 110,
      "is_hidden": 0,
      "is_required": 0,
      "name": "operator_id",
      "no": 2,
      "parent_no": 144,
      "restriction_id": null,
      "template_id": null,
      "title": "操作人id",
      "type_no": 79,
      "unit_id": null
  },
  {
      "description": null,
      "id": 111,
      "is_hidden": 0,
      "is_required": 0,
      "name": "data_type",
      "no": 3,
      "parent_no": 144,
      "restriction_id": null,
      "template_id": null,
      "title": "数据类型",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 112,
      "is_hidden": 0,
      "is_required": 0,
      "name": "data_item",
      "no": 4,
      "parent_no": 144,
      "restriction_id": null,
      "template_id": null,
      "title": "数据项",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 113,
      "is_hidden": 1,
      "is_required": 0,
      "name": "action",
      "no": 5,
      "parent_no": 144,
      "restriction_id": null,
      "template_id": null,
      "title": "动作",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": null,
      "id": 114,
      "is_hidden": 1,
      "is_required": 0,
      "name": "abstract",
      "no": 6,
      "parent_no": 144,
      "restriction_id": null,
      "template_id": null,
      "title": "摘要",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": null,
      "id": 115,
      "is_hidden": 0,
      "is_required": 0,
      "name": "description",
      "no": 7,
      "parent_no": 144,
      "restriction_id": null,
      "template_id": null,
      "title": "备注",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 116,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 145,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 117,
      "is_hidden": 1,
      "is_required": 0,
      "name": "template_id",
      "no": 1,
      "parent_no": 145,
      "restriction_id": null,
      "template_id": null,
      "title": "模板id",
      "type_no": 66,
      "unit_id": null
  },
  {
      "description": null,
      "id": 118,
      "is_hidden": 0,
      "is_required": 0,
      "name": "type",
      "no": 2,
      "parent_no": 145,
      "restriction_id": null,
      "template_id": null,
      "title": "类型",
      "type_no": 264,
      "unit_id": null
  },
  {
      "description": null,
      "id": 119,
      "is_hidden": 0,
      "is_required": 0,
      "name": "name",
      "no": 3,
      "parent_no": 145,
      "restriction_id": null,
      "template_id": null,
      "title": "名称",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"name\", \"filter\": \"\\\"type\\\" = 1\"}",
      "id": 120,
      "is_hidden": 0,
      "is_required": 0,
      "name": "factory_id",
      "no": 4,
      "parent_no": 145,
      "restriction_id": null,
      "template_id": null,
      "title": "工厂id",
      "type_no": 78,
      "unit_id": null
  },
  {
      "description": null,
      "id": 121,
      "is_hidden": 1,
      "is_required": 0,
      "name": "state",
      "no": 5,
      "parent_no": 145,
      "restriction_id": null,
      "template_id": null,
      "title": "状态",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 122,
      "is_hidden": 0,
      "is_required": 0,
      "name": "description",
      "no": 6,
      "parent_no": 145,
      "restriction_id": null,
      "template_id": null,
      "title": "备注",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 123,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 146,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"name\"}",
      "id": 124,
      "is_hidden": 1,
      "is_required": 0,
      "name": "model_id",
      "no": 1,
      "parent_no": 146,
      "restriction_id": null,
      "template_id": null,
      "title": "机型id",
      "type_no": 81,
      "unit_id": null
  },
  {
      "description": null,
      "id": 125,
      "is_hidden": 0,
      "is_required": 0,
      "name": "no",
      "no": 2,
      "parent_no": 146,
      "restriction_id": null,
      "template_id": null,
      "title": "编号",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 126,
      "is_hidden": 1,
      "is_required": 0,
      "name": "state",
      "no": 3,
      "parent_no": 146,
      "restriction_id": null,
      "template_id": null,
      "title": "状态",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 127,
      "is_hidden": 0,
      "is_required": 0,
      "name": "description",
      "no": 4,
      "parent_no": 146,
      "restriction_id": null,
      "template_id": null,
      "title": "备注",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 128,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 147,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"no\"}",
      "id": 129,
      "is_hidden": 1,
      "is_required": 0,
      "name": "state_id",
      "no": 1,
      "parent_no": 147,
      "restriction_id": null,
      "template_id": null,
      "title": "状态id",
      "type_no": 82,
      "unit_id": null
  },
  {
      "description": null,
      "id": 130,
      "is_hidden": 0,
      "is_required": 0,
      "name": "no",
      "no": 2,
      "parent_no": 147,
      "restriction_id": null,
      "template_id": null,
      "title": "编号",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 131,
      "is_hidden": 0,
      "is_required": 0,
      "name": "air_no_range",
      "no": 3,
      "parent_no": 147,
      "restriction_id": null,
      "template_id": null,
      "title": "飞机编号范围",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 132,
      "is_hidden": 0,
      "is_required": 0,
      "name": "engine_no_range",
      "no": 4,
      "parent_no": 147,
      "restriction_id": null,
      "template_id": null,
      "title": "发动机编号范围",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 133,
      "is_hidden": 1,
      "is_required": 0,
      "name": "state",
      "no": 5,
      "parent_no": 147,
      "restriction_id": null,
      "template_id": null,
      "title": "状态",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 134,
      "is_hidden": 0,
      "is_required": 0,
      "name": "description",
      "no": 6,
      "parent_no": 147,
      "restriction_id": null,
      "template_id": null,
      "title": "备注",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 135,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 148,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"no\"}",
      "id": 136,
      "is_hidden": 1,
      "is_required": 0,
      "name": "batch_id",
      "no": 1,
      "parent_no": 149,
      "restriction_id": null,
      "template_id": null,
      "title": "批次id",
      "type_no": 83,
      "unit_id": null
  },
  {
      "description": null,
      "id": 137,
      "is_hidden": 0,
      "is_required": 0,
      "name": "tag",
      "no": 2,
      "parent_no": 148,
      "restriction_id": null,
      "template_id": null,
      "title": "tag",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 138,
      "is_hidden": 0,
      "is_required": 0,
      "name": "no",
      "no": 3,
      "parent_no": 148,
      "restriction_id": null,
      "template_id": null,
      "title": "编号",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"name\", \"filter\": \"\\\"type\\\" = 1\"}",
      "id": 139,
      "is_hidden": 0,
      "is_required": 0,
      "name": "factory_id",
      "no": 4,
      "parent_no": 148,
      "restriction_id": null,
      "template_id": null,
      "title": "承制单位id",
      "type_no": 81,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"name\", \"filter\": \"\\\"type\\\" = 2\"}",
      "id": 140,
      "is_hidden": 1,
      "is_required": 0,
      "name": "user_id",
      "no": 5,
      "parent_no": 148,
      "restriction_id": null,
      "template_id": null,
      "title": "接装单位id",
      "type_no": 81,
      "unit_id": null
  },
  {
      "description": null,
      "id": 141,
      "is_hidden": 0,
      "is_required": 0,
      "name": "offer_time",
      "no": 6,
      "parent_no": 148,
      "restriction_id": null,
      "template_id": null,
      "title": "移交时间",
      "type_no": 6,
      "unit_id": null
  },
  {
      "description": null,
      "id": 142,
      "is_hidden": 0,
      "is_required": 0,
      "name": "offer_no",
      "no": 7,
      "parent_no": 148,
      "restriction_id": null,
      "template_id": null,
      "title": "移交编号",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 143,
      "is_hidden": 0,
      "is_required": 0,
      "name": "lifespan",
      "no": 8,
      "parent_no": 148,
      "restriction_id": null,
      "template_id": null,
      "title": "使用寿命",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 144,
      "is_hidden": 0,
      "is_required": 0,
      "name": "residue_span",
      "no": 9,
      "parent_no": 148,
      "restriction_id": null,
      "template_id": null,
      "title": "剩余寿命",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 145,
      "is_hidden": 0,
      "is_required": 0,
      "name": "engine_model",
      "no": 10,
      "parent_no": 148,
      "restriction_id": null,
      "template_id": null,
      "title": "引擎型号",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 146,
      "is_hidden": 0,
      "is_required": 0,
      "name": "engine_no",
      "no": 11,
      "parent_no": 148,
      "restriction_id": null,
      "template_id": null,
      "title": "引擎编号",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 147,
      "is_hidden": 0,
      "is_required": 0,
      "name": "engine_lifespan",
      "no": 12,
      "parent_no": 148,
      "restriction_id": null,
      "template_id": null,
      "title": "引擎使用寿命",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 148,
      "is_hidden": 0,
      "is_required": 0,
      "name": "engine_residue_span",
      "no": 13,
      "parent_no": 148,
      "restriction_id": null,
      "template_id": null,
      "title": "引擎剩余寿命",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 149,
      "is_hidden": 1,
      "is_required": 0,
      "name": "state",
      "no": 14,
      "parent_no": 148,
      "restriction_id": null,
      "template_id": null,
      "title": "状态",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": null,
      "id": 150,
      "is_hidden": 0,
      "is_required": 0,
      "name": "description",
      "no": 15,
      "parent_no": 148,
      "restriction_id": null,
      "template_id": null,
      "title": "备注",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": "主键",
      "id": 151,
      "is_hidden": 1,
      "is_required": 1,
      "name": "id",
      "no": 0,
      "parent_no": 149,
      "restriction_id": null,
      "template_id": null,
      "title": "id",
      "type_no": 4,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"name\"}",
      "id": 152,
      "is_hidden": 1,
      "is_required": 0,
      "name": "baseline_id",
      "no": 1,
      "parent_no": 149,
      "restriction_id": null,
      "template_id": null,
      "title": "基线id",
      "type_no": 75,
      "unit_id": null
  },
  {
      "description": null,
      "id": 153,
      "is_hidden": 0,
      "is_required": 0,
      "name": "name",
      "no": 2,
      "parent_no": 149,
      "restriction_id": null,
      "template_id": null,
      "title": "名称",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 154,
      "is_hidden": 0,
      "is_required": 0,
      "name": "no",
      "no": 3,
      "parent_no": 149,
      "restriction_id": null,
      "template_id": null,
      "title": "编号",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 155,
      "is_hidden": 0,
      "is_required": 0,
      "name": "object",
      "no": 4,
      "parent_no": 149,
      "restriction_id": null,
      "template_id": null,
      "title": "对象",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 156,
      "is_hidden": 1,
      "is_required": 0,
      "name": "abstract",
      "no": 5,
      "parent_no": 149,
      "restriction_id": null,
      "template_id": null,
      "title": "摘要说明",
      "type_no": 2,
      "unit_id": null
  },
  {
      "description": null,
      "id": 157,
      "is_hidden": 0,
      "is_required": 0,
      "name": "operator",
      "no": 6,
      "parent_no": 149,
      "restriction_id": null,
      "template_id": null,
      "title": "经手人",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 158,
      "is_hidden": 0,
      "is_required": 0,
      "name": "time",
      "no": 7,
      "parent_no": 149,
      "restriction_id": null,
      "template_id": null,
      "title": "时间",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": "{\"value\": \"id\", \"show\": \"name\"}",
      "id": 159,
      "is_hidden": 0,
      "is_required": 0,
      "name": "control_file_id",
      "no": 8,
      "parent_no": 149,
      "restriction_id": null,
      "template_id": null,
      "title": "控制文件id",
      "type_no": 1,
      "unit_id": null
  },
  {
      "description": null,
      "id": 160,
      "is_hidden": 0,
      "is_required": 0,
      "name": "description",
      "no": 15,
      "parent_no": 149,
      "restriction_id": null,
      "template_id": null,
      "title": "备注",
      "type_no": 2,
      "unit_id": null
  }
]
// id, object_type, object_id, previous_version_id
const baselines = []
// id, info_config_id, baseline_id, parent_id, ref_id, serial_no, data
const dataCollection = []
// id, type, name, ext, content, size, digest
const files = []

// id, parent_id, no, name, type, address, telephone, contact, manager_id, description
const organizations = [
  {
    id: 1,
    parent_id: null,
    no: null,
    name: null,
    type: 0,
    address: null,
    telephone: null,
    contact: null,
    manager_id: 1,
    description: null
  }
]
// id, login_name, name, sex, birthday, organization_id, address, telephone, email, website, password, role, description
const users = [
  {
    id: 1,
    login_name: 'admin',
    name: null,
    sex: 0,
    birthday: null,
    organization_id: 1,
    address: null,
    telephone: null,
    email: null,
    website: null,
    password: 'admin123',
    role: '管理员',
    description: null
  }
]
// id, time, operator_id, data_type, data_item, action, abstract, description
const logs = []
// id, template_id, type, name, factory_id, state, description
const models = []
// id, model_id, no, state, description
const states = []
// id, state_id, no, air_no_range, engine_no_range, state, description
const batchs = []
// id, batch_id, tag, no, factory_id, use_organization_id, offer_time, offer_no, lifespan, residue_span, engine_model, engine_no, engine_lifespan, engine_residue_span, state, description
const airs = []
// id, baseline_id, name, no, object, abstract, operator, time, control_file_id, description
const histories = []

const filter = (collection, conditions) => {
  let result = collection
  const conditionCount = conditions.length
  for (let i = 0; i < conditionCount; ++i) {
    if (conditions[i] !== '') {
      const operators = conditions[i].match(/!=|<=|>=|=|>|<| LIKE | is /g)
      if (!operators) {
        continue
      }
      const operator = operators[0]
      const pair = conditions[i].split(operator)
      let name = pair[0].trim()
      if (name[0] === '"' && name[name.length - 1] === '"') {
        name = name.substring(1, name.length - 1)
      }
      let value = pair[1].trim()
      if (value[0] === '\'' && value[value.length - 1] === '\'') {
        value = value.substring(1, value.length - 1)
      }
      result = result.filter(item => {
        const objectValue = item[name]
        let r = false
        switch (operator) {
          case '=':
            r = String(objectValue) === value
            break
          case '!=':
            r = String(objectValue) !== value
            break
          case '<':
            r = Number(objectValue) < Number(value)
            break
          case '>':
            r = Number(objectValue) > Number(value)
            break
          case '<=':
            r = Number(objectValue) <= Number(value)
            break
          case '>=':
            r = Number(objectValue) >= Number(value)
            break
          case ' is ':
            r = objectValue === null
            break
          case ' LIKE ':
            if (value.length) {
              const originValue = String(objectValue)
              if (value[0] === '%') {
                if (value[value.length - 1] === '%') {
                  r = originValue.includes(value.substring(1, value.length - 1))
                } else {
                  r = originValue.endsWith(value.substring(1))
                }
              } else {
                if (value[value.length - 1] === '%') {
                  r = originValue.startsWith(value.substring(0, value.length - 1))
                } else {
                  r = originValue === value
                }
              }
            }
            break
        }
        return r
      })
    }
  }
  return result
}

const orderBy = (collection, orders) => {
  // FIXME
  if (orders.length > 0) {
    const firstOrder = orders[0]
    if (firstOrder !== '') {
      const p = firstOrder.split(' ')
      let column = firstOrder
      let direction = 'ASC'
      if (p.length > 1) {
        column = p[0].trim()
        direction = p[1].trim().toUpperCase()
      }
      if (column[0] === '"' && column[column.length - 1] === '"') {
        column = column.substring(1, column.length - 1)
      }
      collection.sort((a, b) => {
        let r = a[column] - b[column]
        if (direction === 'DESC') {
          r = -r
        }
        return r
      })
    }
  }
}

const Select = (tableName, whereCause, order) => {
  let result
  if (typeof db_select === 'function') {
    // dp(`SELECT * FROM ${tableName} WHERE ${whereCause} ORDER BY ${order};`)
    result = db_select(tableName, whereCause, order)
    return JSON.parse(result.replace(/\n/g, '\\n').replace(/\r/g, '\\r'))
  } else {
    result = []
    const conditions = whereCause.split(' AND ')
    const orders = order.split(',')
    switch (tableName) {
      case 'properties':
        result = filter(properties, conditions)
        break
      case 'templates':
        result = filter(templates, conditions)
        break
      case 'types':
        result = filter(types, conditions)
        break
      case 'restrictions':
        result = filter(restrictions, conditions)
        break
      case 'unit_types':
        result = filter(unitTypes, conditions)
        break
      case 'units':
        result = filter(units, conditions)
        break
      case 'info_configs':
        result = filter(infoConfigs, conditions)
        break
      case 'codes':
        result = filter(codes, conditions)
        break
      case 'schemas':
        result = filter(schemas, conditions)
        break
      case 'lists':
        result = filter(lists, conditions)
        break
      case 'baselines':
        result = filter(baselines, conditions)
        break
      case 'data':
        result = filter(dataCollection, conditions)
        break
      case 'files':
        result = filter(files, conditions)
        break
      case 'organizations':
        result = filter(organizations, conditions)
        break
      case 'users':
        result = filter(users, conditions)
        break
      case 'logs':
        result = filter(logs, conditions)
        break
      case 'models':
        result = filter(models, conditions)
        break
      case 'states':
        result = filter(states, conditions)
        break
      case 'batchs':
        result = filter(batchs, conditions)
        break
      case 'airs':
        result = filter(airs, conditions)
        break
      case 'histories':
        result = filter(histories, conditions)
        break
    }
    orderBy(result, orders)
    return extendDeep([], result, true)
  }
}

const Insert = (tableName, data) => {
  if (typeof db_insert === 'function') {
    // dp(`INSERT INTO ${tableName} (...) VALUES (${JSON.stringify(data)});`)
    return db_insert(tableName, JSON.stringify(data))
  } else {
    if (!data.id) {
      data.id = getNewId(tableName)
    }
    data = extendDeep({}, data)
    switch (tableName) {
      case 'properties':
        properties.push(data)
        break
      case 'templates':
        templates.push(data)
        break
      case 'types':
        types.push(data)
        break
      case 'restrictions':
        restrictions.push(data)
        break
      case 'unit_types':
        unitTypes.push(data)
        break
      case 'units':
        units.push(data)
        break
      case 'info_configs':
        infoConfigs.push(data)
        break
      case 'codes':
        codes.push(data)
        break
      case 'schemas':
        schemas.push(data)
        break
      case 'lists':
        lists.push(data)
        break
      case 'baselines':
        baselines.push(data)
        break
      case 'data':
        dataCollection.push(data)
        break
      case 'files':
        files.push(data)
        break
      case 'logs':
        logs.push(data)
        break
      case 'organizations':
        organizations.push(data)
        break
      case 'users':
        users.push(data)
        break
      case 'models':
        models.push(data)
        break
      case 'states':
        states.push(data)
        break
      case 'batchs':
        batchs.push(data)
        break
      case 'airs':
        airs.push(data)
        break
      case 'histories':
        histories.push(data)
        break
    }
    return data.id
  }
}

const Update = (tableName, data) => {
  if (typeof db_update === 'function') {
    // dp(`UPDATE ${tableName} SET ... = ${JSON.stringify(data)};`)
    db_update(tableName, JSON.stringify(data))
  } else {
    let theData = null
    switch (tableName) {
      case 'properties':
        theData = properties.find(item => item.id === data.id)
        break
      case 'templates':
        theData = templates.find(item => item.id === data.id)
        break
      case 'types':
        theData = types.find(item => item.id === data.id)
        break
      case 'restrictions':
        theData = restrictions.find(item => item.id === data.id)
        break
      case 'unit_types':
        theData = unitTypes.find(item => item.id === data.id)
        break
      case 'units':
        theData = units.find(item => item.id === data.id)
        break
      case 'info_configs':
        theData = infoConfigs.find(item => item.id === data.id)
        break
      case 'codes':
        theData = codes.find(item => item.id === data.id)
        break
      case 'schemas':
        theData = schemas.find(item => item.id === data.id)
        break
      case 'lists':
        theData = lists.find(item => item.id === data.id)
        break
      case 'baselines':
        theData = baselines.find(item => item.id === data.id)
        break
      case 'files':
        theData = files.find(item => item.id === data.id)
        break
      case 'data':
        theData = dataCollection.find(item => item.id === data.id)
        break
      case 'logs':
        theData = logs.find(item => item.id === data.id)
        break
      case 'organizations':
        theData = organizations.find(item => item.id === data.id)
        break
      case 'users':
        theData = users.find(item => item.id === data.id)
        break
      case 'models':
        theData = models.find(item => item.id === data.id)
        break
      case 'states':
        theData = states.find(item => item.id === data.id)
        break
      case 'batchs':
        theData = batchs.find(item => item.id === data.id)
        break
      case 'airs':
        theData = airs.find(item => item.id === data.id)
        break
      case 'histories':
        theData = histories.find(item => item.id === data.id)
        break
    }
    if (theData) {
      for (const propertyName in data) {
        theData[propertyName] = data[propertyName]
      }
    }
  }
}

const Remove = (tableName, id) => {
  if (typeof db_remove === 'function') {
    // dp(`DELETE FROM ${tableName} WHERE id = ${id}`)
    db_remove(tableName, id)
  } else {
    let index = -1
    switch (tableName) {
      case 'properties':
        index = properties.findIndex(item => item.id === id)
        if (index !== -1) {
          properties.splice(index, 1)
        }
        break
      case 'templates':
        index = templates.findIndex(item => item.id === id)
        if (index !== -1) {
          templates.splice(index, 1)
        }
        break
      case 'types':
        index = types.findIndex(item => item.id === id)
        if (index !== -1) {
          types.splice(index, 1)
        }
        break
      case 'restrictions':
        index = restrictions.findIndex(item => item.id === id)
        if (index !== -1) {
          restrictions.splice(index, 1)
        }
        break
      case 'unit_types':
        index = unitTypes.findIndex(item => item.id === id)
        if (index !== -1) {
          unitTypes.splice(index, 1)
        }
        break
      case 'units':
        index = units.findIndex(item => item.id === id)
        if (index !== -1) {
          units.splice(index, 1)
        }
        break
      case 'codes':
        index = codes.findIndex(item => item.id === id)
        if (index !== -1) {
          codes.splice(index, 1)
        }
        break
      case 'schemas':
        index = schemas.findIndex(item => item.id === id)
        if (index !== -1) {
          schemas.splice(index, 1)
        }
        break
      case 'lists':
        index = lists.findIndex(item => item.id === id)
        if (index !== -1) {
          lists.splice(index, 1)
        }
        break
      case 'info_configs':
        index = infoConfigs.findIndex(item => item.id === id)
        if (index !== -1) {
          infoConfigs.splice(index, 1)
        }
        break
      case 'baselines':
        index = baselines.findIndex(item => item.id === id)
        if (index !== -1) {
          baselines.splice(index, 1)
        }
        break
      case 'data':
        index = dataCollection.findIndex(item => item.id === id)
        if (index !== -1) {
          dataCollection.splice(index, 1)
        }
        break
      case 'files':
        index = files.findIndex(item => item.id === id)
        if (index !== -1) {
          files.splice(index, 1)
        }
        break
      case 'logs':
        index = logs.findIndex(item => item.id === id)
        if (index !== -1) {
          logs.splice(index, 1)
        }
        break
      case 'organizations':
        index = organizations.findIndex(item => item.id === id)
        if (index !== -1) {
          organizations.splice(index, 1)
        }
        break
      case 'users':
        index = users.findIndex(item => item.id === id)
        if (index !== -1) {
          users.splice(index, 1)
        }
        break
      case 'models':
        index = models.findIndex(item => item.id === id)
        if (index !== -1) {
          models.splice(index, 1)
        }
        break
      case 'states':
        index = states.findIndex(item => item.id === id)
        if (index !== -1) {
          states.splice(index, 1)
        }
        break
      case 'batchs':
        index = batchs.findIndex(item => item.id === id)
        if (index !== -1) {
          batchs.splice(index, 1)
        }
        break
      case 'airs':
        index = airs.findIndex(item => item.id === id)
        if (index !== -1) {
          airs.splice(index, 1)
        }
        break
      case 'histories':
        index = histories.findIndex(item => item.id === id)
        if (index !== -1) {
          histories.splice(index, 1)
        }
        break
    }
  }
}

const selectOne = (tableName, whereCause) => {
  let result = null
  const r = Select(tableName, whereCause, '')
  if (r && r.length) {
    result = r[0]
  }
  return result
}

const FillFileContent = (tableName, id, columnName, data) => {
  if (typeof db_fillFileContent === 'function') {
    // dp(`UPDATE ${tableName} SET ${columnName} = ${data} WHERE id = ${id};`)
    db_fillFileContent(tableName, id, columnName, data)
  } else {
    let theData = null
    if (tableName === 'files') {
      theData = files.find(item => item.id === id)
      theData.digest = md5.hex_md5(data)
    }
    if (theData) {
      theData[columnName] = data
    }
  }
}

const GetFileContent = (tableName, id, columnName) => {
  if (typeof db_getFileContent === 'function') {
    // dp(`SELECT ${columnName} FROM ${tableName} WHERE id = ${id};`)
    return db_getFileContent(tableName, id, columnName)
  } else {
    let theData = null
    if (tableName === 'files') {
      theData = files.find(item => item.id === id)
      // theData["digest"] = md5.hex_md5(data)
    }
    if (theData) {
      return theData[columnName]
    }
  }
}

const OpenFile = id => {
  if (typeof db_openfile === 'function') {
    db_openfile(id)
  } else {
    console.log('not support')
  }
}

const exportTemplate = templateId => {
  if (typeof db_exportTemplatePackage === 'function') {
    return db_exportTemplatePackage(templateId)
  } else {
    console.log('not support')
  }
  // // read types
  // const result = {
  //   template: selectOne('templates', `id = ${templateId}`),
  //   types: Select('types', `template_id = ${templateId}`, ''),
  //   codes: [],
  //   schemas: [],
  //   lists: []
  // }
  // result.types.forEach(type => {
  //   switch (type.type) {
  //     case 1:
  //       result.codes.push(...Select('codes', `parent_no = type.no`, ''))
  //       break
  //     case 2:
  //       result.schemas.push(...Select('schemas', `parent_no = type.no`, ''))
  //       break
  //     case 3:
  //       result.lists.push(...Select('lists', `parent_no = type.no`, ''))
  //       break
  //   }
  // })
  // result.supportConfigs = Select('support_configs', `template_id = ${templateId}`, '')
  // // write to excel
  // // return result
  // downloadFile(JSON.stringify(result), `template-${templateId}.json`)
}

const exportOrganizations = () => {
  if (typeof db_exportOrganizationsPackage === 'function') {
    return db_exportOrganizationsPackage()
  } else {
    console.log('not support')
  }

  // // read organizations and users
  // const result = {
  //   organizations: Select('组织', '', ''),
  //   users: Select('用户', '', '')
  // }
  // // construct data
  // // write to excel
  // // return result
  // downloadFile(JSON.stringify(result), 'basic-data.json')
}

const exportBatch = batchId => {
  if (typeof db_exportPackage === 'function') {
    return db_exportPackage(0, batchId)
  } else {
    console.log('not support')
  }
  // // read 11 classify data
  // const result = {
  //   batch: selectOne('批次', `id = ${batchId}`),
  //   baseline: selectOne('基线', `"适用目标" = 0 AND "目标id" = ${batchId}`)
  // }
  // result.state = selectOne('状态', `id = ${result.batch['状态id']}`)
  // result.model = selectOne('机型', `id = ${result.state['机型id']}`)
  // result.configurations = Select('构型', `"基线id" = ${result.baseline.id}`, '')
  // result.auxiliaries = Select('随机设备工具', `"基线id" = ${result.baseline.id}`, '')
  // result.machineAux = Select('机械专业随机备件', `"基线id" = ${result.baseline.id}`, '')
  // result.resourceAux = Select('随机资料目录', `"基线id" = ${result.baseline.id}`, '')
  // result.resourceDirectoryAux = Select('随机资料配套目录', `"基线id" = ${result.baseline.id}`, '')
  // result.technologyStateFiles = Select('技术状态文件', `"基线id" = ${result.baseline.id}`, '')
  // result.lifespans = Select('有寿件', `"基线id" = ${result.baseline.id}`, '')
  // result.doubleCurrents = Select('双流水', `"基线id" = ${result.baseline.id}`, '')
  // result.problems = Select('交付遗留问题及处置情况', `"基线id" = ${result.baseline.id}`, '')
  // // construct data
  // // TODO 布局.图片 喷涂方案 技术状态文件s
  // // write to excel
  // console.log(batchId)
}

const exportAir = airId => {
  if (typeof db_exportPackage === 'function') {
    return db_exportPackage(1, airId)
  } else {
    console.log('not support')
  }
  // const result = {
  //   air: selectOne('飞机', `id = ${airId}`),
  //   baseline: selectOne('基线', `"适用目标" = 1 AND "目标id" = ${airId}`)
  // }
  // result.batch = selectOne('批次', `id = ${result.air['批次id']}`)
  // result.state = selectOne('状态', `id = ${result.batch['状态id']}`)
  // result.model = selectOne('机型', `id = ${result.state['机型id']}`)
  // result.configurations = Select('构型', `"基线id" = ${result.baseline.id}`, '')
  // result.auxiliaries = Select('随机设备工具', `"基线id" = ${result.baseline.id}`, '')
  // result.machineAux = Select('机械专业随机备件', `"基线id" = ${result.baseline.id}`, '')
  // result.resourceAux = Select('随机资料目录', `"基线id" = ${result.baseline.id}`, '')
  // result.resourceDirectoryAux = Select('随机资料配套目录', `"基线id" = ${result.baseline.id}`, '')
  // result.technologyStateFiles = Select('技术状态文件', `"基线id" = ${result.baseline.id}`, '')
  // result.lifespans = Select('有寿件', `"基线id" = ${result.baseline.id}`, '')
  // result.doubleCurrents = Select('双流水', `"基线id" = ${result.baseline.id}`, '')
  // result.problems = Select('交付遗留问题及处置情况', `"基线id" = ${result.baseline.id}`, '')
  // // construct data
  // // TODO 布局.图片 喷涂方案 技术状态文件s
  // // write to excel
  // console.log(airId)
}

const importOrganizations = fileContent => {
  if (typeof db_executeSql === 'function') {
    db_executeSql('DELETE FROM "organizations";')
    db_executeSql('DELETE FROM "users";')
  } else {
    const os = Select('organizations', '', '')
    os.forEach(o => Remove('organizations', o.id))
    const us = Select('users', '', '')
    us.forEach(u => Remove('users', u.id))
  }
  // read the excel file
  const r = JSON.parse(fileContent)
  r.organizations.forEach(organization => {
    Insert('organizations', organization)
  })
  r.users.forEach(user => {
    Insert('users', user)
  })
  // write to db
  // console.log(fileContent)
  return 0
}

const importBatch = (fileContent, stateId) => {
  if (typeof db_exportPackage === 'function') {
    return db_importPackage(fileContent, stateId)
  } else {
    console.log('not support')
  }
  // // read the excel file
  // const r = JSON.parse(fileContent)
  // r['state_id'] = stateId
  // // write to db
  // console.log(fileContent, stateId)
}

const importAir = (fileContent, organizationId) => {
  if (typeof db_exportPackage === 'function') {
    return db_importPackage(fileContent, organizationId)
  } else {
    console.log('not support')
  }
  // // read the excel file
  // // write to db
  // console.log(fileContent, organizationId)
}

const ids = {
  properties: 1,
  templates: 2,
  types: 65537,
  restrictions: 1,
  unit_types: 36,
  units: 100,
  info_configs: 1,
  codes: 100,
  schemas: 400,
  lists: 30,
  baselines: 1,
  data: 1,
  files: 1,
  organizations: 2,
  users: 2,
  logs: 1,
  models: 1,
  states: 1,
  batchs: 1,
  airs: 1,
  histories: 1
}

const getNewId = function (tableName) {
  const result = ids[tableName]
  ++ids[tableName]
  return result
}

const getTypeById = id => {
  return selectOne('types', `id = ${id}`)
}

const getModelById = id => {
  return selectOne('models', `id = ${id}`)
}

const getStatesById = id => {
  return selectOne('states', `id = ${id}`)
}

const getBatchById = id => {
  return selectOne('batchs', `id = ${id}`)
}

const getTypeByTemplateIdAndNo = (templateId, no) => {
  let result = selectOne('types', `template_id = ${templateId} AND no = ${no}`)
  if (!result) {
    result = selectOne('types', `template_id is null AND no = ${no}`)
  }
  return result
}

const getTypeDetailTableByNumber = n => {
  let result = ''
  switch (n) {
    case 1:
      result = 'codes'
      break
    case 2:
      result = 'schemas'
      break
    case 3:
      result = 'lists'
      break
  }
  return result
}

const getCodeValues = (templateId, codeNo) => {
  let r = Select('codes', `template_id = ${templateId} AND parent_no = ${codeNo}`, '')
  if (!r || !r.length) {
    r = Select('codes', `template_id is null AND parent_no = ${codeNo}`, '')
  }
  r.sort((a, b) => Number.parseInt(a.no) - Number.parseInt(b.no))
  return r
}

const getSchemaComponents = (templateId, schemaNo) => {
  let r = Select('schemas', `template_id = ${templateId} AND parent_no = ${schemaNo}`, '')
  if (!r || r.length === 0) {
    r = Select('schemas', `template_id is null AND parent_no = ${schemaNo}`, '')
  }
  r.forEach(item => {
    if (!item.restriction || typeof item.restriction === 'string') {
      item.restriction = extendDeep({
        unit: {},
        min: '',
        max: '',
        precision: '',
        format: ''
      }, isJSON(item.restriction) ? JSON.parse(item.restriction) : {})
    }
  })
  r.sort((a, b) => Number.parseInt(a.no) - Number.parseInt(b.no))
  return r
}

const getListElement = (templateId, listNo) => {
  let r = selectOne('lists', `template_id = ${templateId} AND parent_no = ${listNo}`)
  if (!r) {
    r = selectOne('lists', `template_id is null AND parent_no = ${listNo}`)
  }
  return r
}

const getTableSchema = (templateId, listNo) => {
  const result = getListElement(templateId, listNo)
  result.element_type = getTypeByTemplateIdAndNo(templateId, result.element_type_no)
  result.element_type.children = getSchemaComponents(templateId, result.element_type_no)
  return result
}

const groupRows = (rows, groupColumn) => {
  const groupNames = {}
  rows.forEach(row => {
    const groupName = row[groupColumn]
    // if (!groupNames.hasOwnProperty(groupName)) {
    if (!(groupName in groupNames)) {
      groupNames[groupName] = []
    }
    groupNames[groupName].push(row)
  })
  return groupNames
}

const simpleClone = function simpleClone (origin) {
  const assign = function (obj, name, value) {
    if (typeof value === 'object' && value !== null) {
      obj[name] = simpleClone(value)
    } else {
      obj[name] = value
    }
  }
  let target
  if (typeof origin === 'object') {
    if (Array.isArray(origin)) {
      target = []
      origin.forEach((item, index) => {
        assign(target, index, item)
      })
    } else {
      target = {}
      Object.entries(origin).forEach(item => {
        assign(target, item[0], item[1])
      })
    }
  } else {
    target = origin
  }
  return target
}

const extendDeep = function extendDeep (target, origin, isArray = false) {
  const assign = function (obj, name, value) {
    if (typeof value === 'object' && value !== null) {
      const isArr = Array.isArray(value)
      obj[name] = extendDeep(isArr ? [] : {}, value, isArr)
    } else {
      obj[name] = value
    }
  }
  if (isArray) {
    origin.forEach((item, index) => {
      assign(target, index, item)
    })
  } else {
    Object.entries(origin).forEach(item => {
      assign(target, item[0], item[1])
    })
  }
  return target
}

const stepCloneTemplate = (oldTemplateId, templateName, templateDescription, caller) => {
  const template = {
    name: templateName,
    description: templateDescription,
    state: 0
  }
  const templateId = Insert('templates', template)
  const queue = []
  const oldTypes = Select('types', `template_id = ${oldTemplateId}`, 'id ASC')
  oldTypes.forEach(oldType => {
    const type = extendDeep({}, oldType)
    delete type.id
    type.template_id = templateId
    queue.push({
      name: 'types',
      value: type
    })
    // Insert('types', type)
    const detailTable = getTypeDetailTableByNumber(Number.parseInt(oldType.type))
    const oldDetails = Select(detailTable, `template_id = ${oldTemplateId} AND parent_no = ${oldType.no}`, detailTable === 'lists' ? '' : 'no ASC')
    oldDetails.forEach(oldDetail => {
      const detail = extendDeep({}, oldDetail)
      // detail.id = `${getNewId(detailTable)}`
      delete detail.id
      detail.template_id = templateId
      queue.push({
        name: detailTable,
        value: detail
      })
      // Insert(detailTable, detail)
    })
  })

  const oldInfoConfigs = Select('info_configs', `template_id = ${oldTemplateId}`, '')
  oldInfoConfigs.forEach(oldInfoConfig => {
    const supportConfig = extendDeep({}, oldInfoConfig)
    delete supportConfig.id
    supportConfig.template_id = templateId
    queue.push({
      name: 'info_configs',
      value: supportConfig
    })
    // Insert('info_configs', supportConfig)
  })
  const handle = window.requestInterval(() => {
    if (queue.length) {
      const item = queue.shift()
      Insert(item.name, item.value)
    } else {
      window.clearRequestInterval(handle)
      if (caller) {
        caller(templateId)
      }
    }
  }, 16)
}

const cloneTemplate = (oldTemplateId, templateName, templateDescription) => {
  const template = {
    name: templateName,
    description: templateDescription,
    state: 0
  }
  const templateId = Insert('templates', template)

  const oldTypes = Select('types', `template_id = ${oldTemplateId}`, 'id ASC')
  oldTypes.forEach(oldType => {
    /* {
        id: `${getNewId('types')}`,
        no: oldType.no,
        template_id: `${templateId}`,
        name: oldType.name,
        type: oldType.type,
        description: oldType.description
      } */
    const type = extendDeep({}, oldType)
    delete type.id
    type.template_id = templateId
    Insert('types', type)
    const detailTable = getTypeDetailTableByNumber(Number.parseInt(type.type))
    const oldDetails = Select(detailTable, `template_id = ${oldTemplateId} AND parent_no = ${type.no}`, detailTable === 'lists' ? '' : 'no ASC')
    let command = `INSERT INTO ${detailTable} (template_id, parent_id, name, table_no) VALUES `
    switch (type.type) {
      case 1:
        command = 'INSERT INTO codes (template_id, parent_no, no, value, description) VALUES '
        break
      case 2:
        command = 'INSERT INTO schemas (template_id, parent_no, no, name, type_no, is_required, restriction, description) VALUES '
        break
      case 3:
        command = 'INSERT INTO lists (template_id, parent_no, element_type_no, min, max, group_by, is_hidden, description) VALUES '
        break
    }
    let isFirst = true
    oldDetails.forEach(oldDetail => {
      if (isFirst) {
        isFirst = false
      } else {
        command += ', '
      }
      const sqlQuoteString = v => v ? "'" + v + "'" : 'NULL'
      switch (type.type) {
        case 1:
          command += `(${templateId}, ${oldDetail.parent_no}, ${oldDetail.no}, '${oldDetail.value}', ${sqlQuoteString(oldDetail.description)})`
          break
        case 2:
          command += `(${templateId}, ${oldDetail.parent_no}, ${oldDetail.no}, '${oldDetail.name}', ${oldDetail.type_no}, ${oldDetail.is_required}, ${sqlQuoteString(oldDetail.restriction)}, ${sqlQuoteString(oldDetail.description)})`
          // command += `(${templateId}, ${oldDetail.parent_no}, ${oldDetail.no}, '${oldDetail.name}', ${oldDetail.type_no}, ${oldDetail.is_required}, ${sqlQuoteString(oldDetail.restriction)}, ${sqlQuoteString(oldDetail.description)})`
          break
        case 3:
          command += `(${templateId}, ${oldDetail.parent_no}, ${oldDetail.element_type_no}, ${oldDetail.min}, ${oldDetail.max}, ${sqlQuoteString(oldDetail.group_by)}, ${oldDetail.is_hidden}, ${sqlQuoteString(oldDetail.description)})`
          break
      }
      // if (typeof db_executeSql !== 'function') {
      const detail = extendDeep({}, oldDetail)
      // detail.id = `${getNewId(detailTable)}`
      delete detail.id
      detail.template_id = templateId
      Insert(detailTable, detail)
      // }
    })
    if (typeof db_executeSql === 'function') {
      // db_executeSql(command)
    }
  })

  let command = 'INSERT INTO info_configs (template_id, parent_id, name, table_no) VALUES '
  let isFirst = true
  const oldSupportConfigs = Select('info_configs', `template_id = ${oldTemplateId}`, '')
  oldSupportConfigs.forEach(oldSupportConfig => {
    if (isFirst) {
      isFirst = false
    } else {
      command += ', '
    }
    command += `(${templateId}, ${oldSupportConfig.parent_id}, '${oldSupportConfig.name}', ${oldSupportConfig.table_no})`
    if (typeof db_executeSql !== 'function') {
      const supportConfig = extendDeep({}, oldSupportConfig)
      delete supportConfig.id
      supportConfig.template_id = templateId
      Insert('support_configs', supportConfig)
    }
  })
  if (typeof db_executeSql === 'function') {
    db_executeSql(command)
  }
  return templateId
}

const cloneBaseLine = (oldBaseLineId, objectType, objectId) => {
  const baseline = {
    object_type: objectType,
    object_id: objectId
  }
  const baseLineId = Insert('baselines', baseline)

  const oldData = Select('data', `baseline_id = ${oldBaseLineId}`, 'id ASC')
  const idMaps = {}
  oldData.forEach(oldDatum => {
    const data = extendDeep({}, oldDatum)
    delete data.id
    data.baseline_id = baseLineId
    if (oldDatum.parent_id !== null) {
      data.parent_id = idMaps[oldDatum.parent_id]
    }
    idMaps[oldDatum.id] = Insert('data', data)
  })

  return baseLineId
}

const moveTechnologyStateFiles = (configurationId, newBaselineId) => {
  const files = Select('data', `"ref_id" = ${configurationId}`, '')
  for (const file of files) {
    file.baseline_id = newBaselineId
    Update('data', file)
  }
}

const moveConfigurationNode = (nodeId, newParentId) => {
  const move = (self, newBaseLineId) => {
    self.baseline_id = newBaseLineId
    Update('data', self)
    moveTechnologyStateFiles(self.id, newBaseLineId)
    const children = Select('data', `parent_id = ${self.id}`, '')
    for (const child of children) {
      move(child, newBaseLineId)
    }
  }
  const self = selectOne('data', `id = ${nodeId}`)
  self.parent_id = newParentId
  const newParent = selectOne('data', `id = ${newParentId}`)
  const newBaselineId = newParent.baseline_id
  move(self, newBaselineId)
}

const copyTechnologyStateFiles = (configurationId, newBaselineId, newConfigurationId) => {
  const files = Select('data', `"ref_id" = ${configurationId}`, '')
  for (const file of files) {
    file.baseline_id = newBaselineId
    file.ref_id = newConfigurationId
    Insert('data', file)
  }
}

const copyConfigurationNode = (nodeId, newParentId) => {
  const copy = (self, newBaseLineId, newParentId) => {
    const newSelf = extendDeep({}, self)
    delete newSelf.id
    newSelf.baseline_id = newBaseLineId
    newSelf.parent_id = newParentId
    const newId = Insert('data', newSelf)
    copyTechnologyStateFiles(self.id, newBaseLineId, newId)
    const children = Select('data', `parent_id = ${self.id}`, '')
    for (const child of children) {
      copy(child, newBaseLineId, newId)
    }
  }
  const self = selectOne('data', `id = ${nodeId}`)
  const newParent = selectOne('data', `id = ${newParentId}`)
  const newBaselineId = newParent.baseline_id
  copy(self, newBaselineId, newParentId)
}

const removeTechnologyStateFiles = configurationId => {
  const files = Select('data', `"ref_id" = ${configurationId}`, '')
  for (const file of files) {
    Remove('data', file.id)
  }
}

const removeConfigurationNode = nodeId => {
  const remove = nodeId => {
    const children = Select('data', `parent_id = ${nodeId}`, '')
    for (const child of children) {
      remove(child.id)
    }
    Remove('data', nodeId)
    removeTechnologyStateFiles(nodeId)
  }
  remove(nodeId)
}

const getTopTables = () => {
  const r = Select('types', 'template_id is null AND type = 3', '')
  r.sort((a, b) => Number.parseInt(a.id) - Number.parseInt(b.id))
  return r
}

const tables = getTopTables()

const isTable = type => {
  return tables.find(item => item.id === type.id) !== undefined
}

const getTableTypeNo = tableName => {
  let result = 0
  const table = tables.find(item => item.name === tableName)
  if (table) {
    result = table.no
  }
  return result
}

const tableNos = {
  properties: 65,
  templates: 66,
  types: 67,
  restrictions: 68,
  unit_types: 69,
  units: 70,
  info_configs: 71,
  codes: 72,
  schemas: 73,
  lists: 74,
  baselines: 75,
  data: 76,
  files: 77,
  organizations: 78,
  users: 79,
  logs: 80,
  models: 81,
  states: 82,
  batchs: 83,
  airs: 84,
  histories: 85
}

const construct = (templateId, parentNo, value) => {
  const result = []
  const columns = getSchemaComponents(templateId, parentNo)
  columns.forEach(column => {
    result.push({
      name: column.name,
      relation: column,
      type: getTypeByTemplateIdAndNo(templateId, column.type_no),
      value: value[column.name]
    })
  })
  return result
}

const removeUnneedFields = (defines, fields) => {
  if (fields && fields.length) {
    for (let i = 0; i < defines.length; ++i) {
      if (fields.indexOf(defines[i].name) !== -1) {
        defines.splice(i, 1)
        --i
      }
    }
  } else {
    for (let i = 0; i < defines.length; ++i) {
      if (defines[i].type.type === 3) {
        if (isTable(defines[i].type)) {
          defines.splice(i, 1)
          --i
        }
      } else if (defines[i].name === 'id') {
        defines.splice(i, 1)
        --i
      }
    }
  }
  return defines
}

const tableCaches = {}

const processForeignKey = defines => {
  /*
  for (let i = 0; i < defines.length; ++i) {
    if (defines[i].type.type === 3) {
      if (isTable(defines[i].type)) {
        defines.splice(i, 1)
        --i
      }
    }
  }
  return defines
  */
  defines.forEach(define => {
    if (define.type.type === 3) {
      if (isTable(define.type)) {
        if (define.name !== 'parent_id') {
          define.isForeignKey = true
          define.title = define.name.replace('id', '')
          if (define.schema.description) {
            define.desc = JSON.parse(define.schema.description)
          }
          if (!Object.prototype.hasOwnProperty.call(tableCaches, define.type.name)) {
            tableCaches[define.type.name] = Select(define.type.name, '', '')
          }
          let r = Select(define.type.name, '', '') // tableCaches[define.type.name]
          if (define.desc) {
            if (define.desc.filter) {
              r = filter(r, define.desc.filter.split(' AND '))
            }
            if (define.desc.show) {
              r = r.map(item => ({
                id: item.id,
                value: item[define.desc.show]
              }))
            }
          }
          define.valueSet = r
        }
      }
    }
  })
  return defines
}

const processEnums = defines => {
  defines.forEach(define => {
    if (define.type.type === 1) {
      define.children = getCodeValues(define.type.template_id, define.type.no)
    }
  })
  return defines
}

const processComposites = defines => {
  // const defineCount = defines.length
  for (let i = 0; i < defines.length; ++i) {
    if (defines[i].type.type === 2) {
      const children = construct(defines[i].type.template_id, defines[i].type.no, defines[i].value || {})
      if (children && children.length) {
        defines[i].children = children
      } else {
        defines.splice(i, 1)
        --i
      }
    }
  }
  return defines
  /*
  const indexComponents = {}
  const defineCount = defines.length
  for (let i = 0; i< defineCount; ++i) {
    if (defines[i].type.type == '2') {
      indexComponents[i] = construct(defines[i].type.template_id, defines[i].type.no, defines[i].value || {})
    }
  }
  let difference = 0
  for (const index in indexComponents) {
    defines.splice(Number(index) + difference, 1, ...indexComponents[index])
    difference += indexComponents[index].length - 1
  }
  return defines
  */
}

const processTemplate = defines => {
  const defineCount = defines.length
  for (let i = 0; i < defineCount; ++i) {
    const define = defines[i]
    if (define.name === 'template_id') {
      if (define.value) {
        define.name = '模板'
        define.value = Select('templates', `id = ${define.value}`, '')[0].name
        define.disabled = true
      }
      break
    }
  }
  return defines
}

const processState = defines => {
  const defineCount = defines.length
  for (let i = 0; i < defineCount; ++i) {
    if (defines[i].name === 'state') {
      defines.splice(i, 1)
      break
    }
  }
  return defines
}

const processId = defines => {
  const defineCount = defines.length
  for (let i = 0; i < defineCount; ++i) {
    if (defines[i].name === 'id') {
      defines.splice(i, 1)
      break
    }
  }
  return defines
}

const getExtended = (templateId, tableName, value) => {
  let result = null
  const tableNo = tableNos[tableName]
  if (tableNo) {
    const list = getListElement('null', tableNo)
    if (list) {
      const columns = getSchemaComponents('null', list.element_type_no)
      const extended = columns.find(column => column.name === 'extended')
      if (extended) {
        result = construct(templateId, extended.type_no, value)
      }
    }
  }
  return result
}

const render = (templateId, parentNo, value) => {
  return processComposites(processForeignKey(processEnums(removeUnneedFields(construct(templateId, parentNo, value), ['id', 'template_id', 'state']))))
  // return processComposites(processForeignKey(processEnums(processTemplate(processId(construct(templateId, parentNo, value))))))
}

const equals = (left, right) => {
  let result = true
  const leftEntries = left.entries()
  const rightEntries = right.entries()
  if (leftEntries.length === rightEntries.length) {
    for (let i = 0; i < leftEntries.length; ++i) {
      const leftEntry = leftEntries[i]
      const rightEntry = rightEntries.find(entry => entry[0] === leftEntry[0])
      if (rightEntry) {
        if (leftEntry[1] !== rightEntry[1]) {
          result = false
          break
        }
      } else {
        result = false
        break
      }
    }
  } else {
    result = false
  }
  return result
}

const canOperation = (nodeId, newId) => {
  const childrenIncluded = (nodeId, newId) => {
    let result = false
    const children = Select('data', `parent_id = ${nodeId}`, '')
    for (const child of children) {
      if (child.id === newId) {
        result = true
        break
      }
      result = childrenIncluded(child.id, newId)
    }
    return result
  }

  let result
  if (nodeId === newId) {
    result = false
  } else {
    result = !childrenIncluded(nodeId, newId)
  }
  return result
}

const compareList = (templateId, schemaNo, lists) => {
  const result = {
    isSame: true,
    data: []
  }
  const totalListCount = lists.length
  let maxLength = lists[0].length
  for (let i = 1; i < totalListCount; ++i) {
    if (lists[i].length > maxLength) {
      maxLength = lists[i].length
      result.isSame = false
    }
  }
  const fieldNames = []
  for (const component of getSchemaComponents(templateId, schemaNo)) {
    fieldNames.push(component.name)
    const type = getTypeByTemplateIdAndNo(templateId, component.type_no)
    if (type.type === 2) {
      for (const subComponent of getSchemaComponents(templateId, component.type_no)) {
        fieldNames.push(component.name + '-' + subComponent.name)
      }
    }
  }
  const fetchFields = (obj, fieldNames) => {
    const result = []
    for (const fieldName of fieldNames) {
      let originValue = obj[fieldName]
      const field = {
        name: fieldName,
        isSame: true,
        data: [originValue]
      }
      const path = fieldName.split('-')
      if (path.length !== 1) {
        originValue = obj[path[0]]
        if (typeof originValue === 'string' && isJSON(originValue)) {
          originValue = JSON.parse(originValue)
        }
        const subField = {
          name: fieldName,
          isSame: true,
          data: [originValue[path[1]]]
        }
        result.push(subField)
      }
      result.push(field)
    }
    return result
  }
  const compareObject = (fields, right) => {
    let result = true
    for (const field of fields) {
      const path = field.name.split('-')
      if (path.length === 1) {
        if (field.value[0] !== right[field.name]) {
          field.isSame = false
        }
        field.data.push(right[field.name])
      } else {
        let originValue = right[path[0]]
        if (typeof originValue === 'string' && isJSON(originValue)) {
          originValue = JSON.parse(originValue)
        }
        if (field.data.value[0] !== originValue[path[1]]) {
          field.isSame = false
          fields.find(f => f.name === path[0]).isSame = false
          result = false
        }
        field.data.push([originValue[path[1]]])
      }
    }
    return result
  }
  const sameRows = []
  for (let i = 0; i < maxLength; ++i) {
    for (const list of lists) {
      if (list.length < i) {
        sameRows.push(list[i])
      } else {
        sameRows.push(null)
      }
    }
  }
  for (const sameRow of sameRows) {
    const rowResult = {
      isSame: true,
      data: fetchFields(sameRow[0], fieldNames)
    }
    for (let i = 1; i < totalListCount; ++i) {
      const r = compareObject(rowResult.data, sameRow[i])
      if (!r) {
        rowResult.isSame = false
      }
      rowResult.data.push(sameRow[i])
    }
    result.data.push(rowResult)
    if (!rowResult.isSame) {
      result.isSame = false
    }
  }
  return result
}

const compare = (templateId, batchs, airs) => {
  const compares = getTableSchema(templateId, tableNos.batchs)
  const result = []
  // const baselines = []
  // const types = []
  compares.forEach(item => {
    const line = {
      name: item,
      isSame: true,
      values: []
    }
    switch (item) {
      case '使命任务':
        do {
          batchs.forEach(batch => {
            const state = selectOne('状态', `id = ${batch['状态id']}`)
            line.values.push(state['使命任务'])
          })
          airs.forEach(air => {
            const batch = selectOne('批次', `id = ${air.data['批次id']}`)
            const state = selectOne('状态', `id = ${batch['状态id']}`)
            line.values.push(state['使命任务'])
          })
          const value = line.values[0]
          for (let index = 1; index < line.values.length; ++index) {
            if (value !== line.values[index]) {
              line.isSame = false
              break
            }
          }
          result.push(line)
        } while (false)
        break
      case '布局':
        do {
          batchs.forEach(batch => {
            line.values.push(JSON.parse(batch[item]))
          })
          airs.forEach(air => {
            const batch = selectOne('批次', `id = ${air['批次id']}`)
            line.values.push(JSON.parse(batch[item]))
          })
          result.push(line)
          const components = Select('schemas', `template_id = ${templateId} AND parent_no = 262`, 'no ASC')
          for (const component of components) {
            const subLine = {
              name: `${item}.${component.name}`,
              isSame: true,
              values: []
            }
            for (let i = 0; i < line.values.length; ++i) {
              subLine.values.push(line.values[i][component.name]) // getTypeByTemplateIdAndNo(templateId, component.type_no).name
            }
            const value = subLine.values[0]
            for (let index = 1; index < subLine.values.length; ++index) {
              if (value !== subLine.values[index]) {
                subLine.isSame = false
                line.isSame = false
                break
              }
            }
            result.push(subLine)
          }
        } while (false)
        break
      case '主要技术参数':
        do {
          batchs.forEach(batch => {
            line.values.push(JSON.parse(batch[item]))
          })
          airs.forEach(air => {
            const batch = selectOne('批次', `id = ${air['批次id']}`)
            line.values.push(JSON.parse(batch[item]))
          })
          result.push(line)
          const components = Select('schemas', `template_id = ${templateId} AND parent_no = 263`, 'no ASC')
          for (const component of components) {
            const subLine = {
              name: `${item}.${component.name}`,
              isSame: true,
              values: []
            }
            for (let i = 0; i < line.values.length; ++i) {
              subLine.values.push(line.values[i][component.name]) // getTypeByTemplateIdAndNo(templateId, component.type_no).name
            }
            result.push(subLine)
            const subComponents = Select('schemas', `template_id = ${templateId} AND parent_no = ${component.type_no}`, 'no ASC')
            for (const subComponent of subComponents) {
              const subSubLine = {
                name: `${item}.${component.name}.${subComponent.name}`,
                isSame: true,
                values: []
              }
              for (let j = 0; j < subLine.values.length; ++j) {
                subSubLine.values.push(subLine.values[j][subComponent.name])
              }
              const value = subSubLine.values[0]
              for (let index = 1; index < subSubLine.values.length; ++index) {
                if (value !== subSubLine.values[index]) {
                  subSubLine.isSame = false
                  subLine.isSame = false
                  line.isSame = false
                  break
                }
              }
              result.push(subSubLine)
            }
          }
        } while (false)
        break
    }
  })
  return result
}

const writeExcel = (name, sheets) => {
  const wb = XLSX.utils.book_new()
  for (const sheet of sheets) {
    const ws = XLSX.utils.aoa_to_sheet(sheet.data)
    XLSX.utils.book_append_sheet(wb, ws, sheet.name)
  }
  /* generate file and send to client */
  XLSX.writeFile(wb, name + '.xlsx')
}

const exportExcel = (templateId, tableTypeNo) => {
  const type = getListElement(templateId, tableTypeNo)
  if (type !== null) {
    const elementType = getTypeByTemplateIdAndNo(templateId, type.element_type_no)
    if (elementType !== null) {
      const row = []
      const components = getSchemaComponents(templateId, type.element_type_no)
      switch (elementType.type) {
        case 2:
          for (const component of components) {
            row.push(component.name)
          }
          writeExcel('template', [
            {
              name: 'sheet1',
              data: [row]
            }
          ])
          break
        case 1:
        case 0:
          break
        default:
          break
      }
    }
  }
  return 0
}

const importExcel = fileContent => {
  const result = []
  const workbook = XLSX.read(fileContent, { type: 'binary' })
  workbook.SheetNames.forEach(sheetName => {
    const range = workbook.Sheets[sheetName]['!ref']
    if (range) {
      if (range.includes(':')) {
        const [first, second] = range.split(':')
        const pattern = /([A-Z]+)([0-9]+)/
        const [begin, beginColumn, beginRow] = first.match(pattern)
        const [end, endColumn, endRow] = second.match(pattern)
        const lastRow = Number.parseInt(endRow) + 1
        const lastColumn = columnIncreament(endColumn)
        const names = []
        let isFirst = true
        for (let row = Number.parseInt(beginRow); row < lastRow; ++row) {
          if (isFirst) {
            for (let column = beginColumn; column !== lastColumn; column = columnIncreament(column)) {
              const cellName = column + row
              const contents = workbook.Sheets[sheetName][cellName]
              if (contents && contents.v) {
                names.push(contents.v)
              }
            }
            isFirst = false
          } else {
            const rowItem = {}
            let columnIndex = 0
            for (let column = beginColumn; column !== lastColumn; column = columnIncreament(column)) {
              const cellName = column + row
              const contents = workbook.Sheets[sheetName][cellName]
              if (contents && contents.v) {
                const v = contents.v
                if (v !== '') {
                  rowItem[names[columnIndex]] = v
                }
              }
              ++columnIndex
            }
            result.push(rowItem)
          }
        }
      } else {
        console.log('excel file content error!')
        const contents = workbook.Sheets[sheetName][range]
        if (contents && contents.v) {
          const v = contents.v
          if (v !== '') {
            console.log(sheetName + ':' + range + ':' + v)
          }
        }
      }
    }
  })
  return result
}

const importTemplate = fileContent => {
  // read the excel file
  const r = JSON.parse(fileContent)
  Insert('templates', r.template)
  r.types.forEach(type => {
    Insert('types', type)
  })
  r.codes.forEach(code => {
    Insert('codes', code)
  })
  r.schemas.forEach(schema => {
    Insert('schemas', schema)
  })
  r.lists.forEach(list => {
    Insert('lists', list)
  })
  r.supportConfigs.forEach(supportConfig => {
    Insert('info_configs', supportConfig)
  })
  // write to db
  // console.log(fileContent)
  return 0
}

const columnIncreament = col => {
  const firstLetter = 'A'.charCodeAt(0)
  const lastLetter = 'Z'.charCodeAt(0)
  const colCodes = []
  for (let i = 0; i < col.length; ++i) {
    colCodes.push(col.charCodeAt(i))
  }
  let widen = false
  for (let i = colCodes.length - 1; i >= 0; --i) {
    if (colCodes[i] === lastLetter) {
      colCodes[i] = firstLetter
    } else {
      ++colCodes[i]
      break
    }
    if (i === 0) {
      widen = true
    }
  }
  return widen ? String.fromCharCode(firstLetter, ...colCodes) : String.fromCharCode(...colCodes)
}

const importConfigurations = (fileContent, rootId) => {
  let baselineId = null
  const root = selectOne('data', `id = ${rootId}`)
  if (root) {
    baselineId = root.baseline_id
  }
  const workbook = XLSX.read(fileContent, { type: 'binary' })
  workbook.SheetNames.forEach(sheetName => {
    const range = workbook.Sheets[sheetName]['!ref']
    if (range.includes(':')) {
      const [first, second] = range.split(':')
      const pattern = /([A-Z]+)([0-9]+)/
      const [begin, beginColumn, beginRow] = first.match(pattern)
      const [end, endColumn, endRow] = second.match(pattern)
      const lastRow = Number.parseInt(endRow) + 1
      const lastColumn = columnIncreament(endColumn)
      const idChain = [rootId]
      // 编号 名称 型号 单机定额 装机部位 /*承制单位*/ 重要度 状态 主管 版本 备注 扩展1
      let columnIndex = 0
      for (let row = Number.parseInt(beginRow); row < lastRow; ++row) {
        columnIndex = 0
        let serialNo
        let name
        const extended = {}
        for (let column = beginColumn; column !== lastColumn; column = columnIncreament(column)) {
          const cellName = column + row
          const contents = workbook.Sheets[sheetName][cellName]
          if (contents && contents.v) {
            const v = contents.v
            if (v !== '') {
              if (v === '编号') {
                // the header, skip this row
                break
              } else {
                switch (columnIndex) {
                  case 0:
                    serialNo = String(v)
                    extended.serialNo = String(v)
                    break
                  case 1:
                    name = String(v)
                    extended.name = String(v)
                    break
                  case 2:
                    extended.model = String(v)
                    break
                  case 3:
                    extended.count = String(v)
                    break
                  case 4:
                    extended.position = String(v)
                    break
                  // case 5:
                  //   organization = String(v)
                  //   break
                  case 5:
                    extended.importantLevel = String(v)
                    break
                  case 6:
                    extended.state = String(v)
                    break
                  case 7:
                    extended.manager = String(v)
                    break
                  case 8:
                    extended.version = String(v)
                    break
                  case 9:
                    extended.description = String(v)
                    break
                  default:
                    const [propName, propValue] = String(v).split(':')
                    extended[propName] = propValue
                    break
                }
              }
              console.log(sheetName + ':' + cellName + ':' + v)
            }
          }
          ++columnIndex
        }
        if (serialNo && name) {
          // process line
          const serialParts = serialNo.split('.')
          idChain.length = serialParts.length
          const item = {
            baseline_id: baselineId,
            parent_id: idChain[idChain.length - 1],
            // 名称: name,
            // 型号: model,
            // 单机定额: count,
            // 装机部位: position,
            // // '承制单位id': organization,
            // 重要度: importantLevel,
            // 状态: state,
            // 主管: manager,
            // 版本: version,
            // 备注: description,
            data: JSON.stringify(extended)
          }
          idChain.push(Insert('data', item))
        }
      }
    } else {
      console.log('excel file content error!')
      const contents = workbook.Sheets[sheetName][range]
      if (contents && contents.v) {
        const v = contents.v
        if (v !== '') {
          console.log(sheetName + ':' + range + ':' + v)
        }
      }
    }
  })
  return 0
}

const importConfigurationNames = (fileContent, rootId) => {
  let baselineId = null
  const root = selectOne('data', `id = ${rootId}`)
  if (root) {
    baselineId = root.baseline_id
  }
  const workbook = XLSX.read(fileContent, { type: 'binary' })
  workbook.SheetNames.forEach(sheetName => {
    const range = workbook.Sheets[sheetName]['!ref']
    if (range.includes(':')) {
      const [first, second] = range.split(':')
      const pattern = /([A-Z]+)([0-9]+)/
      const [begin, beginColumn, beginRow] = first.match(pattern)
      const [end, endColumn, endRow] = second.match(pattern)
      const lastRow = Number.parseInt(endRow) + 1
      const lastColumn = columnIncreament(endColumn)
      const idChain = [rootId]
      // 编号 名称 型号 单机定额 装机部位 /*承制单位*/ 重要度 状态 主管 版本 备注 扩展1
      let columnIndex = 0
      for (let row = Number.parseInt(beginRow); row < lastRow; ++row) {
        columnIndex = 0
        let serialNo
        let system = null
        let divideSystem = null
        let subSystem = null
        let device = null
        const extended = {}
        for (let column = beginColumn; column !== lastColumn; column = columnIncreament(column)) {
          const cellName = column + row
          const contents = workbook.Sheets[sheetName][cellName]
          if (contents && contents.v) {
            const v = contents.v
            if (v !== '') {
              if (v === '序号') {
                // 序号 系统 分系统 子系统 零部件/设备 软件配置项 主管 备注 扩展1
                // the header, skip this row
                break
              } else {
                switch (columnIndex) {
                  case 0:
                    serialNo = String(v)
                    extended.serialNo = String(v)
                    break
                  case 1:
                    system = String(v)
                    extended.system = String(v)
                    break
                  case 2:
                    divideSystem = String(v)
                    extended.divideSystem = String(v)
                    break
                  case 3:
                    subSystem = String(v)
                    extended.subSystem = String(v)
                    break
                  case 4:
                    device = String(v)
                    extended.device = String(v)
                    break
                  case 5:
                    extended.software = String(v)
                    break
                  case 6:
                    extended.manager = String(v)
                    break
                  case 7:
                    extended.description = String(v)
                    break
                  case 8:
                    extended.extended1 = String(v)
                    break
                }
              }
              console.log(sheetName + ':' + cellName + ':' + v)
            }
          }
          ++columnIndex
        }
        if (serialNo) {
          let level = 1
          let name
          // process line
          if (system) {
            level = 1
            name = system
          } else if (divideSystem) {
            level = 2
            name = divideSystem
          } else if (subSystem) {
            level = 3
            name = subSystem
          } else if (device) {
            level = 4
            name = device
          }
          extended.name = name
          idChain.length = level
          const item = {
            baseline_id: baselineId,
            parent_id: idChain[idChain.length - 1],
            data: JSON.stringify(extended)
            // 名称: name,
            // 重要度: software,
            // 主管: manager,
            // 备注: description
          }
          idChain.push(Insert('data', item))
        }
      }
    } else {
      console.log('excel file content error!')
      const contents = workbook.Sheets[sheetName][range]
      if (contents && contents.v) {
        const v = contents.v
        if (v !== '') {
          console.log(sheetName + ':' + range + ':' + v)
        }
      }
    }
  })
  return 0
}

const getAncestors = (templateId, typeNo) => {
  const result = []
  const lists = Select('lists', `template_id = ${templateId} AND element_type_no = ${typeNo}`, '')
  const allTypes = Select('types', `template_id = ${templateId}`, '')
  result.push(...allTypes.filter(type => {
    let result = false
    for (const list of lists) {
      if (type.no === list.parent_no) {
        result = true
        break
      }
    }
    return result
  }))
  const schemas = Select('schemas', `template_id = ${templateId} AND type_no = ${typeNo}`, '')
  const ancestors = []
  schemas.forEach(schema => {
    ancestors.push(...Select('schemas', `template_id = ${templateId} AND type_no = ${schema.parent_no}`, ''))
  })
  schemas.push(...ancestors)
  result.push(...allTypes.filter(type => {
    let result = false
    for (const schema of schemas) {
      if (type.no === schema.parent_no) {
        result = true
        break
      }
    }
    return result
  }))
  return result
}

const filterTypes = (templateId, parentTypeNo) => {
  const result = []
  const type = getTypeByTemplateIdAndNo(templateId, parentTypeNo)
  if (type.type === 2) {
    result.push(...Select('types', 'template_id is null AND type = 0', ''))
    result.push(...Select('types', `template_id = ${templateId} AND type = 1`, ''))
    result.push(...Select('types', `template_id = ${templateId} AND type = 3`, ''))
    const sts = Select('types', `template_id = ${templateId} AND type = 2`, '')
    result.push(...sts.filter(st => {
      let result = true
      if (type.no === parentTypeNo) {
        result = false
      } else {
        const ancestors = getAncestors(st)
        for (const t of ancestors) {
          if (t.no === st.no) {
            result = false
            break
          }
        }
      }
      return result
    }))
  } else if (type.type === 3) {
    result.push(...Select('types', 'template_id is null AND type = 0', ''))
    result.push(...Select('types', `template_id = ${templateId} AND type = 1`, ''))
    result.push(...Select('types', `template_id = ${templateId} AND type = 2`, ''))
  }
  return result
}

function downloadFile (content, filename) {
  const a = document.createElement('a')
  const blob = new window.Blob([content])
  const url = window.URL.createObjectURL(blob)
  a.href = url
  a.download = filename
  a.click()
  window.URL.revokeObjectURL(url)
}

const isJSON = str => {
  if (typeof str === 'string') {
    try {
      const obj = JSON.parse(str)
      return !!(typeof obj === 'object' && obj);
    } catch (e) {
      return false
    }
  }
}

export default {
  Select,
  Insert,
  Update,
  FillFileContent,
  GetFileContent,
  OpenFile,
  Remove,
  getBatchById,
  getModelById,
  getStatesById,
  getCodeValues,
  getSchemaComponents,
  getListElement,
  getTableSchema,
  groupRows,
  equals,
  canOperation,
  construct,
  processForeignKey,
  removeUnneedFields,
  processEnums,
  processComposites,
  processTemplate,
  processId,
  processState,
  getTypeById,
  getTypeDetailTableByNumber,
  stepCloneTemplate,
  cloneTemplate,
  cloneBaseLine,
  moveConfigurationNode,
  copyConfigurationNode,
  removeConfigurationNode,
  importConfigurations,
  getTableTypeNo,
  filterTypes,
  tableNos,
  getExtended,
  render,
  getTypeByTemplateIdAndNo,
  compareList,
  compare,
  getNewId,
  selectOne,
  exportTemplate,
  exportOrganizations,
  exportBatch,
  exportAir,
  importTemplate,
  importOrganizations,
  importConfigurationNames,
  importBatch,
  importAir,
  simpleClone,
  extendDeep,
  downloadFile,
  isJSON,
  exportExcel,
  importExcel
}

window.requestInterval = function (fn, delay) {
  let start = new Date().getTime()
  const handle = {}

  function loop () {
    handle.value = window.requestAnimationFrame(loop)
    const current = new Date().getTime()
    const delta = current - start
    if (delta >= delay) {
      fn.call()
      start = new Date().getTime()
    }
  }

  handle.value = window.requestAnimationFrame(loop)
  return handle
}

window.clearRequestInterval = function (handle) {
  window.cancelAnimationFrame(handle.value)
}
