CREATE TABLE properties (
    id integer PRIMARY KEY,
    name text,
    value text
);

CREATE TABLE templates (
    id integer PRIMARY KEY,
    name text,
    state integer,
    description text
);

INSERT INTO templates (id, name, state, description) VALUES
(1, '基础模板', 0, '基本类型设计');

CREATE TABLE types (
    id integer PRIMARY KEY, -- builtin type start from 1 to 256, user define type start 257 to 65536
    template_id integer REFERENCES templates (id),
    no integer, -- builtin type start from 1 to 256(1-64, 65-256), user define type start 257 to 65536
    name text, -- base type: CHARACTER VARYING, TEXT, NUMERIC, INTEGER, REAL[, DATE, TIME], BLOB, IMAGE, NULL
    type integer, -- 0: raw 1: codes 2: schemas 3: lists
    description text -- for builtin type(Numeric), the region, the unit, the precision
);

INSERT INTO types (id, no, template_id, name, type, description) VALUES
(NULL, 1, NULL, '文本', 0, '单行文本'),
(NULL, 2, NULL, '长文本', 0, '可换行文本'),
(NULL, 3, NULL, '数字', 0, '整数或者实数、可以规定小数位数'),
(NULL, 4, NULL, '整数', 0, '整数'),
(NULL, 5, NULL, '实数', 0, '实数'),
(NULL, 6, NULL, '日期', 0, '年月日'),
(NULL, 7, NULL, '时间', 0, '时分秒'),
(NULL, 8, NULL, '数据对象', 0, '二进制内容'),
(NULL, 9, NULL, '图片', 0, '图片内容'),

(NULL, 65, NULL, 'properties', 3, '所有的配置'),
(NULL, 66, NULL, 'templates', 3, '所有的模板'),
(NULL, 67, NULL, 'types', 3, '所有的类型'),
(NULL, 68, NULL, 'restrictions', 3, '所有的限制'),
(NULL, 69, NULL, 'unit_types', 3, '所有的单位范畴'),
(NULL, 70, NULL, 'units', 3, '所有的单位'),
(NULL, 71, NULL, 'info_configs', 3, '所有的信息组织'),
(NULL, 72, NULL, 'codes', 3, '所有的枚举'),
(NULL, 73, NULL, 'schemas', 3, '所有的复合'),
(NULL, 74, NULL, 'lists', 3, '所有的列表'),
(NULL, 75, NULL, 'baselines', 3, '所有的基线'),
(NULL, 76, NULL, 'data', 3, '所有的数据'),
(NULL, 77, NULL, 'files', 3, '所有的文件'),
(NULL, 78, NULL, 'organizations', 3, '所有的组织'),
(NULL, 79, NULL, 'users', 3, '所有的用户'),
(NULL, 80, NULL, 'logs', 3, '所有的日志'),
(NULL, 81, NULL, 'models', 3, '所有的机型'),
(NULL, 82, NULL, 'states', 3, '所有的状态'),
(NULL, 83, NULL, 'batchs', 3, '所有的批次'),
(NULL, 84, NULL, 'airs', 3, '所有的飞机'),
(NULL, 85, NULL, 'histories', 3, '所有的变更'),

(NULL, 129, NULL, 'properties项类型', 3, '配置的属性'),
(NULL, 120, NULL, 'templates项类型', 3, '模板的属性'),
(NULL, 131, NULL, 'types项类型', 3, '类型的属性'),
(NULL, 132, NULL, 'restrictions项类型', 3, '限制的属性'),
(NULL, 133, NULL, 'unit_types项类型', 3, '单位范畴的属性'),
(NULL, 134, NULL, 'units项类型', 3, '单位的属性'),
(NULL, 135, NULL, 'info_configs项类型', 3, '信息组织的属性'),
(NULL, 136, NULL, 'codes项类型', 3, '枚举的属性'),
(NULL, 137, NULL, 'schemas项类型', 3, '复合的属性'),
(NULL, 138, NULL, 'lists项类型', 3, '列表的属性'),
(NULL, 139, NULL, 'baselines项类型', 3, '基线的属性'),
(NULL, 140, NULL, 'data项类型', 3, '数据的属性'),
(NULL, 141, NULL, 'files项类型', 3, '文件的属性'),
(NULL, 142, NULL, 'organizations项类型', 2, '单位的属性'),
(NULL, 143, NULL, 'users项类型', 2, '用户的属性'),
(NULL, 144, NULL, 'logs项类型', 2, '日志的属性'),
(NULL, 145, NULL, 'models项类型', 2, '机型的属性'),
(NULL, 146, NULL, 'states项类型', 2, '状态的属性'),
(NULL, 147, NULL, 'batchs项类型', 2, '批次的属性'),
(NULL, 148, NULL, 'airs项类型', 2, '飞机的属性'),
(NULL, 149, NULL, 'histories项类型', 2, '变更的属性'),

(NULL, 257, NULL, 'types__type', 1, NULL),
(NULL, 258, NULL, 'info_configs__type', 1, NULL),
(NULL, 259, NULL, 'schemas__is_required', 1, NULL),
(NULL, 260, NULL, 'schemas__is_hidden', 1, NULL),
(NULL, 261, NULL, 'baselines__object_type', 1, NULL),

(NULL, 262, 1, 'organizations__type', 1, NULL),
(NULL, 263, 1, 'users__sex', 1, NULL),
(NULL, 264, 1, 'models__type', 1, NULL);

CREATE TABLE restrictions (
    id integer PRIMARY KEY,
    template_id integer REFERENCES templates (id),
    length integer,
    format text,
    precision integer,
    min real,
    max real
);

CREATE TABLE unit_types (
    id integer PRIMARY KEY,
    name text,
    description text
);

INSERT INTO unit_types (id, name, description) VALUES
(1, '量词', NULL),
(2, '长度', NULL),
(3, '面积', NULL),
(4, '体积', NULL),
(5, '质量', NULL),
(6, '时间', NULL),
(7, '平面角', NULL),
(8, '密度', NULL),
(9, '压力', NULL),
(10, '力', NULL),
(11, '温度', NULL),
(12, '传导系数', NULL),
(13, '比热容', NULL),
(14, '能量、功、热', NULL),
(15, '功率、辐射能量', NULL),
(16, '速度', NULL),
(17, '电容', NULL),
(18, '电阻', NULL),
(19, '流量', NULL),
(20, '频率', NULL),
(21, '电荷量', NULL),
(22, '电势、电压、电动势', NULL),
(23, '电导', NULL),
(24, '磁通量', NULL),
(25, '磁通量密度', NULL),
(26, '电感', NULL),
(27, '光通量', NULL),
(28, '光照度', NULL),
(29, '放射性活度', NULL),
(30, '吸收当量', NULL),
(31, '剂量当量', NULL),
(32, '旋转速度', NULL),
(33, '能', NULL),
(34, '级差', NULL),
(35, '线密度', NULL);

CREATE TABLE units (
    id integer PRIMARY KEY,
    type_id text REFERENCES unit_types (id),
    name text,
    symbol text,
    description text
);

INSERT INTO units (id, type_id, name, symbol, description) VALUES
(1, 1, '个', '个', NULL),
(2, 1, '件', '件', NULL),
(3, 1, '辆', '辆', NULL),
(4, 1, '架', '架', NULL),
(5, 1, '台', '台', NULL),
(6, 1, '片', '片', NULL),
(7, 1, '颗', '颗', NULL),
(8, 1, '粒', '粒', NULL),
(9, 1, '门', '门', NULL),
(10, 1, '只', '只', NULL),
(11, 1, '双', '双', NULL),
(12, 1, '棵', '棵', NULL),
(13, 1, '张', '张', NULL),
(14, 1, '串', '串', NULL),
(15, 1, '把', '把', NULL),
(16, 1, '群', '群', NULL),
(17, 1, '本', '本', NULL),
(18, 1, '匹', '匹', NULL),
(19, 1, '条', '条', NULL),
(20, 1, '部', '部', NULL),
(21, 1, '艘', '艘', NULL),
(22, 1, '间', '间', NULL),
(23, 1, '束', '束', NULL),
(24, 1, '幅', '幅', NULL),
(25, 1, '种', '种', NULL),
(26, 1, '杯', '杯', NULL),
(27, 2, '千米', 'Km', NULL),
(28, 2, '米', 'm', NULL),
(29, 2, '分米', 'dm', NULL),
(30, 2, '厘米', 'cm', NULL),
(31, 2, '毫米', 'mm', NULL),
(32, 2, '微米', 'um', NULL),
(33, 2, '海里', 'mile', NULL),
(34, 3, '平方千米', 'Km2', NULL),
(35, 3, '平方米', 'm2', NULL),
(36, 3, '平方分米', 'dm2', NULL),
(37, 3, '平方厘米', 'cm2', NULL),
(38, 3, '平方毫米', 'mm2', NULL),
(39, 3, '平方微米', 'um2', NULL),
(40, 3, '公顷', 'ha', NULL),
(41, 4, '立方千米', 'Km3', NULL),
(42, 4, '立方米', 'm3', NULL),
(43, 4, '立方分米', 'dm3', NULL),
(44, 4, '立方厘米', 'cm3', NULL),
(45, 4, '立方毫米', 'mm3', NULL),
(46, 4, '立方微米', 'um3', NULL),
(47, 4, '升', 'L', NULL),
(48, 4, '毫升', 'mL', NULL),
(49, 5, '吨', 't', NULL),
(50, 5, '千克', 'Kg', NULL),
(51, 5, '克', 'g', NULL),
(52, 6, '秒', 's', NULL),
(53, 6, '分', 'min', NULL),
(54, 6, '小时', 'h', NULL),
(55, 7, '秒', '“', NULL),
(56, 7, '分', '‘', NULL),
(57, 7, '度', '˚', NULL),
(58, 8, '千克/立方米', 'Kg/m3', NULL),
(59, 8, '克/立方厘米', 'g/cm3', NULL),
(60, 9, '兆帕', 'Mpa', NULL),
(61, 9, '帕', 'Pa', NULL),
(62, 9, '巴', 'b', NULL),
(63, 9, '大气压', 'at m', NULL),
(64, 10, '力', 'N', NULL),
(65, 11, '开尔文度', 'K', NULL),
(66, 11, '摄氏度', '℃', NULL),
(67, 11, '华氏度', '℉', NULL),
(68, 12, '千卡', 'kcal/m2.h.℃', NULL),
(69, 13, '千卡/千克.℃', 'kcal/kg.℃', NULL),
(70, 14, '焦耳', 'J', NULL),
(71, 14, '卡', 'cal', NULL),
(72, 14, '千瓦时', 'kW.h', NULL),
(73, 15, '千瓦', 'kW', NULL),
(74, 15, '瓦', 'W', NULL),
(75, 15, '马力', 'hp', NULL),
(76, 16, '千米/小时', 'km/h', NULL),
(77, 16, '米/秒', 'm/s', NULL),
(78, 16, '节', 'kn', NULL),
(79, 17, '法', 'F', NULL),
(80, 17, '微法', 'mF', NULL),
(81, 17, '皮法', 'pF', NULL),
(82, 18, '欧姆', 'Ω', NULL),(83, 19, '立方米/秒', '(m3/s)', NULL),
(84, 20, '赫兹', 'Hz', NULL),
(85, 21, '库伦', 'C', NULL),
(86, 22, '伏特', 'V', NULL),
(87, 23, '西门子', 'S', NULL),
(88, 24, '韦伯', 'Wb', NULL),
(89, 25, '特斯拉', 'T', NULL),
(90, 26, '亨利', 'H', NULL),
(91, 27, '流明', 'lm', NULL),
(92, 28, '勒克斯', 'lx', NULL),
(93, 29, '贝可', 'Bq', NULL),
(94, 30, '戈瑞', 'Gy', NULL),
(95, 31, '希沃特', 'Sv', NULL),
(96, 32, '转/分', 'r/min', NULL),
(97, 33, '电子伏', 'eV', NULL),
(98, 34, '分贝', 'dB', NULL),
(99, 35, '特克斯', 'tex', NULL);

CREATE TABLE info_configs (
    id integer PRIMARY KEY,
    template_id integer REFERENCES templates (id),
    parent_id integer REFERENCES info_configs (id),
    type integer, -- 0: package | 1: item
    no integer,
    name text,
    type_no integer, -- REFERENCES types (no)
    reference_to integer REFERENCES info_configs (id),
    is_required integer, -- 0: should, 1: must
    restriction_id integer REFERENCES restrictions (id),
    unit_id integer REFERENCES units (id),
    description text
);

CREATE TABLE codes (
    id integer PRIMARY KEY,
    template_id integer REFERENCES templates (id),
    parent_no integer, -- REFERENCES types (no),
    no integer,
    value text,
    description text
);

INSERT INTO codes (id, template_id, parent_no, no, value, description) VALUES
(NULL, NULL, 257, 0, '原始类型', NULL),
(NULL, NULL, 257, 1, '枚举类型', NULL),
(NULL, NULL, 257, 2, '复合类型', NULL),
(NULL, NULL, 257, 3, '列表类型', NULL),

(NULL, NULL, 258, 0, '数据包', NULL),
(NULL, NULL, 258, 1, '数据项', NULL),

(NULL, NULL, 259, 0, '选填', NULL),
(NULL, NULL, 259, 1, '必填', NULL),

(NULL, NULL, 260, 0, '显示', NULL),
(NULL, NULL, 260, 1, '隐藏', NULL),

(NULL, NULL, 261, 0, '批次', NULL),
(NULL, NULL, 261, 1, '单机', NULL),

(NULL, 1, 262, 0, '管理单位', '用于设计数据类型并分析数据差异的单位'),
(NULL, 1, 262, 1, '承制单位', '用于填报数据的单位'),
(NULL, 1, 262, 2, '接装单位', '用于维护单机技术状态并进行状态变更的单位'),

(NULL, 1, 263, 0, '女', NULL),
(NULL, 1, 263, 1, '男', NULL),

(NULL, 1, 264, 0, '歼击机', NULL),
(NULL, 1, 264, 1, '强击机', NULL),
(NULL, 1, 264, 2, '轰炸机', NULL),
(NULL, 1, 264, 3, '运输机', NULL),
(NULL, 1, 264, 5, '直升机', NULL),
(NULL, 1, 264, 6, '教练机', NULL),
(NULL, 1, 264, 7, '特种机', NULL),
(NULL, 1, 264, 8, '无人机', NULL);

CREATE TABLE schemas (
    id integer PRIMARY KEY,
    template_id integer REFERENCES templates (id),
    parent_no integer, -- REFERENCES types (no),
    no integer,
    name text,
    title text,
    is_hidden integer, -- 0: show, 1: hide
    type_no integer, -- REFERENCES types (no),
    is_required integer, -- 0: should, 1: must
    restriction_id integer REFERENCES restrictions (id),
    unit_id integer REFERENCES units (id),
    description text -- foreign-key properties
);

INSERT INTO schemas (id, template_id, parent_no, no, name, title, is_hidden, type_no, is_required, restriction_id, unit_id, description) VALUES
-- property
(NULL, NULL, 129, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 129, 1, 'name', '名称', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 129, 2, 'value', '值', 0, 2, 0, NULL, NULL, NULL),
-- template
(NULL, NULL, 130, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 130, 1, 'name', '名称', 0, 1, 1, NULL, NULL, NULL),
(NULL, NULL, 130, 2, 'state', '状态', 1, 4, 1, NULL, NULL, NULL),
(NULL, NULL, 130, 3, 'description', '备注', 0, 2, 0, NULL, NULL, NULL),
-- type
(NULL, NULL, 131, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 131, 1, 'no', '编号', 0, 1, 1, NULL, NULL, NULL),
(NULL, NULL, 131, 2, 'template_id', '模板id', 1, 66, 0, NULL, NULL, '{"value": "id", "show": "name"}'),
(NULL, NULL, 131, 3, 'name', '名称', 0, 1, 1, NULL, NULL, NULL),
(NULL, NULL, 131, 4, 'type', '类型', 0, 257, 1, NULL, NULL, NULL),
(NULL, NULL, 131, 5, 'description', '备注', 1, 2, 0, NULL, NULL, NULL),
-- restriction
(NULL, NULL, 132, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 132, 1, 'template_id', '模板id', 1, 66, 1, NULL, NULL, '{"value": "id", "show": "name"}'),
(NULL, NULL, 132, 2, 'length', '长度', 0, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 132, 3, 'format', '格式', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 132, 4, 'precision', '精度', 0, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 132, 5, 'min', '最小值', 0, 5, 0, NULL, NULL, NULL),
(NULL, NULL, 132, 6, 'max', '最大值', 1, 5, 0, NULL, NULL, NULL),
-- unit_type
(NULL, NULL, 133, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 133, 1, 'name', '名称', 0, 1, 1, NULL, NULL, NULL),
(NULL, NULL, 133, 2, 'description', '备注', 0, 2, 0, NULL, NULL, NULL),
-- unit
(NULL, NULL, 134, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 134, 1, 'type_id', '范畴id', 1, 69, 0, NULL, NULL, '{"value": "id", "show": "name"}'),
(NULL, NULL, 134, 2, 'name', '名称', 0, 1, 1, NULL, NULL, NULL),
(NULL, NULL, 134, 3, 'symbol', '符号', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 134, 4, 'description', '备注', 0, 2, 0, NULL, NULL, NULL),
-- info_config
(NULL, NULL, 135, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 135, 1, 'template_id', '模板id', 1, 4, 1, NULL, NULL, '{"value": "id", "show": "name"}'),
(NULL, NULL, 135, 2, 'parent_id', '父id', 0, 71, 0, NULL, NULL, '{"value": "id", "show": "name"}'),
(NULL, NULL, 135, 3, 'type', '类型', 0, 258, 1, NULL, NULL, NULL),
(NULL, NULL, 135, 4, 'no', '序号', 0, 4, 1, NULL, NULL, NULL),
(NULL, NULL, 135, 5, 'name', '名称', 0, 1, 1, NULL, NULL, NULL),
(NULL, NULL, 135, 6, 'type_no', '类型编号', 0, 4, 1, NULL, NULL, NULL),
(NULL, NULL, 135, 7, 'reference_to', '引用到', 0, 71, 0, NULL, NULL, '{"value": "id", "show": "name"}'),
(NULL, NULL, 135, 8, 'is_required', '必填', 0, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 135, 9, 'restriction_id', '限制id', 0, 68, 0, NULL, NULL, '{"value": "id", "show": "id"}'),
(NULL, NULL, 135, 10, 'unit_id', '单位id', 0, 70, 0, NULL, NULL, '{"value": "id", "show": "symbol"}'),
(NULL, NULL, 135, 11, 'description', '备注', 1, 2, 0, NULL, NULL, NULL),
-- code
(NULL, NULL, 136, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 136, 1, 'template_id', '模板id', 1, 66, 1, NULL, NULL, '{"value": "id", "show": "name"}'),
(NULL, NULL, 136, 2, 'parent_no', '父编号', 1, 4, 1, NULL, NULL, NULL),
(NULL, NULL, 136, 3, 'no', '序号', 0, 4, 1, NULL, NULL, NULL),
(NULL, NULL, 136, 4, 'value', '值', 0, 1, 1, NULL, NULL, NULL),
(NULL, NULL, 136, 5, 'description', '备注', 1, 2, 0, NULL, NULL, NULL),
-- schema
(NULL, NULL, 137, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 137, 1, 'template_id', '模板id', 1, 66, 1, NULL, NULL, '{"value": "id", "show": "name"}'),
(NULL, NULL, 137, 2, 'parent_no', '父编号', 1, 4, 1, NULL, NULL, NULL),
(NULL, NULL, 137, 3, 'no', '序号', 1, 4, 1, NULL, NULL, NULL),
(NULL, NULL, 137, 4, 'name', '名称', 0, 1, 1, NULL, NULL, NULL),
(NULL, NULL, 137, 5, 'title', '标题', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 137, 6, 'is_hidden', '隐藏', 0, 4, 1, NULL, NULL, NULL),
(NULL, NULL, 137, 7, 'type_no', '类型编号', 0, 4, 1, NULL, NULL, NULL),
(NULL, NULL, 137, 8, 'is_required', '必填', 0, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 137, 9, 'restriction_id', '限制id', 0, 68, 0, NULL, NULL, '{"value": "id", "show": "id"}'),
(NULL, NULL, 137, 10, 'unit_id', '单位id', 0, 70, 0, NULL, NULL, '{"value": "id", "show": "symbol"}'),
(NULL, NULL, 137, 11, 'description', '备注', 1, 2, 0, NULL, NULL, NULL),
-- list
(NULL, NULL, 138, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 138, 1, 'template_id', '模板id', 1, 66, 1, NULL, NULL, '{"value": "id", "show": "name"}'),
(NULL, NULL, 138, 2, 'parent_no', '父编号', 1, 4, 1, NULL, NULL, NULL),
(NULL, NULL, 138, 3, 'element_type_no', '元素类型编号', 0, 4, 1, NULL, NULL, NULL),
(NULL, NULL, 138, 4, 'tree_show_column_no', '树显示列序号', 0, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 138, 5, 'min', '最小行数', 0, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 138, 6, 'max', '最大行数', 0, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 138, 7, 'group_by', '分组列', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 138, 8, 'description', '备注', 1, 2, 0, NULL, NULL, NULL),
-- baseline
(NULL, NULL, 139, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 139, 1, 'object_type', '适用目标', 1, 261, 0, NULL, NULL, NULL),
(NULL, NULL, 139, 2, 'object_id', '目标id', 1, 4, 0, NULL, NULL, '批次或单机的id'),
(NULL, NULL, 139, 3, 'previous_version_id', '前一版本id', 1, 75, 0, NULL, NULL, '{"value": "id", "show": "id"}'),
-- data
(NULL, NULL, 140, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 140, 1, 'info_config_id', '信息配置id', 71, 1, 0, NULL, NULL, '{"value": "id", "show": "name"}'),
(NULL, NULL, 140, 2, 'baseline_id', '基线id', 0, 75, 0, NULL, NULL, '{"value": "id", "show": "id"}'),
(NULL, NULL, 140, 3, 'parent_id', '父id', 0, 76, 0, NULL, NULL, '{"value": "id", "show": "id"}'),
(NULL, NULL, 140, 4, 'ref_id', '引用id', 0, 76, 0, NULL, NULL, '{"value": "id", "show": "id"}'),
(NULL, NULL, 140, 5, 'serial_no', '序号', 0, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 140, 6, 'data', '数据', 0, 2, 0, NULL, NULL, NULL),
-- file
(NULL, NULL, 141, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 141, 1, 'type', 'mime-type', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 141, 2, 'name', '名称', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 141, 3, 'ext', '扩展名', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 141, 4, 'content', '内容', 0, 8, 0, NULL, NULL, NULL),
(NULL, NULL, 141, 5, 'size', '文件大小', 0, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 141, 6, 'digest', '内容摘要', 0, 1, 0, NULL, NULL, NULL),
-- organization
(NULL, NULL, 142, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 142, 1, 'no', '编号', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 142, 2, 'name', '名称', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 142, 3, 'type', '类型', 1, 262, 0, NULL, NULL, NULL),
(NULL, NULL, 142, 4, 'address', '地址', 1, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 142, 5, 'telephone', '电话', 1, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 142, 6, 'contact', '联系人', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 142, 7, 'manager_id', '管理员id', 1, 79, 0, NULL, NULL, '{"value": "id", "show": "name", "filter": "\"organization_id\" = id"}'),
(NULL, NULL, 142, 8, 'description', '备注', 1, 2, 0, NULL, NULL, NULL),
-- user
(NULL, NULL, 143, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 143, 1, 'login_name', '登录名', 1, 1, 1, NULL, NULL, NULL),
(NULL, NULL, 143, 2, 'name', '姓名', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 143, 3, 'sex', '性别', 1, 263, 0, NULL, NULL, NULL),
(NULL, NULL, 143, 4, 'birthday', '生日', 1, 6, 0, NULL, NULL, NULL),
(NULL, NULL, 143, 5, 'organization_id', '组织id', 1, 78, 0, NULL, NULL, '{"value": "id", "show": "name"}'),
(NULL, NULL, 143, 6, 'address', '住址', 1, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 143, 7, 'telephone', '电话', 1, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 143, 8, 'email', '电子邮箱', 1, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 143, 9, 'website', '网站', 1, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 143, 10, 'password', '密码', 1, 1, 1, NULL, NULL, NULL),
(NULL, NULL, 143, 11, 'role', '角色', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 143, 12, 'description', '备注', 0, 2, 0, NULL, NULL, NULL),
-- log
(NULL, NULL, 144, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 144, 1, 'time', '时间', 1, 6, 0, NULL, NULL, NULL),
(NULL, NULL, 144, 2, 'operator_id', '操作人id', 0, 79, 0, NULL, NULL, '{"value": "id", "show": "name"}'),
(NULL, NULL, 144, 3, 'data_type', '数据类型', 0, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 144, 4, 'data_item', '数据项', 0, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 144, 5, 'action', '动作', 1, 2, 0, NULL, NULL, NULL),
(NULL, NULL, 144, 6, 'abstract', '摘要', 1, 2, 0, NULL, NULL, NULL),
(NULL, NULL, 144, 7, 'description', '备注', 0, 2, 0, NULL, NULL, NULL),
-- model
(NULL, NULL, 145, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 145, 1, 'template_id', '模板id', 1, 66, 0, NULL, NULL, NULL),
(NULL, NULL, 145, 2, 'type', '类型', 0, 264, 0, NULL, NULL, NULL),
(NULL, NULL, 145, 3, 'name', '名称', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 145, 4, 'factory_id', '工厂id', 0, 78, 0, NULL, NULL, '{"value": "id", "show": "name", "filter": "\"type\" = 1"}'),
(NULL, NULL, 145, 5, 'state', '状态', 1, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 145, 6, 'description', '备注', 0, 2, 0, NULL, NULL, NULL),
-- state
(NULL, NULL, 146, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 146, 1, 'model_id', '机型id', 1, 81, 0, NULL, NULL, '{"value": "id", "show": "name"}'),
(NULL, NULL, 146, 2, 'no', '编号', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 146, 3, 'state', '状态', 1, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 146, 4, 'description', '备注', 0, 2, 0, NULL, NULL, NULL),
-- batch
(NULL, NULL, 147, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 147, 1, 'state_id', '状态id', 1, 82, 0, NULL, NULL, '{"value": "id", "show": "no"}'),
(NULL, NULL, 147, 2, 'no', '编号', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 147, 3, 'air_no_range', '飞机编号范围', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 147, 4, 'engine_no_range', '发动机编号范围', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 147, 5, 'state', '状态', 1, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 147, 6, 'description', '备注', 0, 2, 0, NULL, NULL, NULL),
-- air
(NULL, NULL, 148, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 149, 1, 'batch_id', '批次id', 1, 83, 0, NULL, NULL, '{"value": "id", "show": "no"}'),
(NULL, NULL, 148, 2, 'tag', 'tag', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 148, 3, 'no', '编号', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 148, 4, 'factory_id', '承制单位id', 0, 81, 0, NULL, NULL, '{"value": "id", "show": "name", "filter": "\"type\" = 1"}'),
(NULL, NULL, 148, 5, 'user_id', '接装单位id', 1, 81, 0, NULL, NULL, '{"value": "id", "show": "name", "filter": "\"type\" = 2"}'),
(NULL, NULL, 148, 6, 'offer_time', '移交时间', 0, 6, 0, NULL, NULL, NULL),
(NULL, NULL, 148, 7, 'offer_no', '移交编号', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 148, 8, 'lifespan', '使用寿命', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 148, 9, 'residue_span', '剩余寿命', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 148, 10, 'engine_model', '引擎型号', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 148, 11, 'engine_no', '引擎编号', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 148, 12, 'engine_lifespan', '引擎使用寿命', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 148, 13, 'engine_residue_span', '引擎剩余寿命', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 148, 14, 'state', '状态', 1, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 148, 15, 'description', '备注', 0, 2, 0, NULL, NULL, NULL),
-- history
(NULL, NULL, 149, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 149, 1, 'baseline_id', '基线id', 1, 75, 0, NULL, NULL, '{"value": "id", "show": "name"}'),
(NULL, NULL, 149, 2, 'name', '名称', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 149, 3, 'no', '编号', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 149, 4, 'object', '对象', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 149, 5, 'abstract', '摘要说明', 1, 2, 0, NULL, NULL, NULL),
(NULL, NULL, 149, 6, 'operator', '经手人', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 149, 7, 'time', '时间', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 149, 8, 'control_file_id', '控制文件id', 0, 1, 0, NULL, NULL,  '{"value": "id", "show": "name"}'),
(NULL, NULL, 149, 15, 'description', '备注', 0, 2, 0, NULL, NULL, NULL);

CREATE TABLE lists (
    id integer PRIMARY KEY,
    template_id integer REFERENCES templates (id),
    parent_no integer, -- REFERENCES types (no),
    element_type_no integer, -- REFERENCES types (no),
    tree_show_column_no integer, -- -1: not tree, 0..maxColumn: treeShowColumnNo
    min integer, -- 0
    max integer, -- -1无穷 if (最少==最多) then 列表定长
    group_by text, -- column-name
    description text
);

INSERT INTO lists (id, template_id, parent_no, element_type_no, tree_show_column_no, min, max, group_by, description) VALUES
(NULL, NULL, 65, 129, NULL, 0, -1, NULL, NULL), -- properties
(NULL, NULL, 66, 130, NULL, 0, -1, NULL, NULL), -- templates
(NULL, NULL, 67, 131, NULL, 0, -1, NULL, NULL), -- types
(NULL, NULL, 68, 132, NULL, 0, -1, NULL, NULL), -- restrictions
(NULL, NULL, 69, 133, NULL, 0, -1, NULL, NULL), -- unit_types
(NULL, NULL, 70, 134, NULL, 0, -1, NULL, NULL), -- units
(NULL, NULL, 71, 135, NULL, 0, -1, NULL, NULL), -- info_configs
(NULL, NULL, 72, 136, NULL, 0, -1, NULL, NULL), -- codes
(NULL, NULL, 73, 137, NULL, 0, -1, NULL, NULL), -- schemas
(NULL, NULL, 74, 138, NULL, 0, -1, NULL, NULL), -- lists
(NULL, NULL, 75, 139, NULL, 0, -1, NULL, NULL), -- baselines
(NULL, NULL, 76, 140, NULL, 0, -1, NULL, NULL), -- data
(NULL, NULL, 77, 141, NULL, 0, -1, NULL, NULL), -- files
(NULL, NULL, 78, 142, NULL, 0, -1, 'type', NULL), -- organizations
(NULL, NULL, 79, 143, NULL, 0, -1, NULL, NULL), -- users
(NULL, NULL, 80, 144, NULL, 0, -1, NULL, NULL), -- logs
(NULL, NULL, 81, 145, NULL, 0, -1, NULL, NULL), -- models
(NULL, NULL, 82, 146, NULL, 0, -1, NULL, NULL), -- states
(NULL, NULL, 83, 147, NULL, 0, -1, NULL, NULL), -- batchs
(NULL, NULL, 84, 148, NULL, 0, -1, 'tag', NULL), -- airs
(NULL, NULL, 85, 149, NULL, 0, -1, NULL, NULL); -- histories

CREATE TABLE baselines (
    id integer PRIMARY KEY,
    object_type integer, -- 基线适用目标类型, -- 0: 批次 1: 单机
    object_id integer,
    previous_version_id integer REFERENCES baselines (id)
);

CREATE TABLE data (
    id integer PRIMARY KEY,
    info_config_id integer REFERENCES info_configs (id),
    baseline_id integer REFERENCES baselines (id),
    parent_id integer REFERENCES data (id),
    ref_id integer REFERENCES data (id),
    serial_no integer,
    data text
);

CREATE TABLE files (
    id integer PRIMARY KEY,
    type text, -- mime-type
    name text,
    ext text,
    content blob, -- or self-ref-id
    size integer,
    digest text
);

CREATE TABLE organizations (
    id integer PRIMARY KEY,
    parent_id integer REFERENCES organizations (id), -- level1: 大区 level2: 基地 level3: 师 or 旅 level4: 团 or null
    no text,
    name text,
    type integer, -- 组织类型, -- 1: creator 0: manager 2: army
    address text,
    telephone text,
    contact text,
    manager_id integer REFERENCES users (id),
    description text
);

CREATE TABLE users (
    id integer PRIMARY KEY,
    login_name text,
    name text,
    sex integer, -- 性别类型 0: female, 1: male
    birthday text,
    organization_id integer REFERENCES organizations (id),
    address text,
    telephone text,
    email text,
    website text,
    password text,
    role text,
    description text
);

INSERT INTO organizations (id, parent_id, no, name, type, address, telephone, contact, manager_id, description) VALUES
(1, NULL, NULL, '航空兵所', 0, NULL, NULL, NULL, NULL, NULL);

INSERT INTO users (id, login_name, name, sex, birthday, organization_id, address, telephone, email, website, password, role, description) VALUES
(1, 'admin', NULL, '0', NULL, '1', NULL, NULL, NULL, NULL, 'admin123', '管理员', NULL);

UPDATE organizations SET manager_id = 1 WHERE id = 1;

CREATE TABLE logs (
    id integer PRIMARY KEY,
    "time" text,
    operator_id integer REFERENCES users (id),
    data_type integer,
    data_item integer,
    action text,
    abstract text,
    description text
);

CREATE TABLE models (
    id integer PRIMARY KEY,
    template_id integer,
    type integer, -- 飞机类型, -- 轰炸机 歼击机 运输机 ……
    name text,
    factory_id integer REFERENCES organizations (id),
    state integer,
    description text
);

CREATE TABLE states (
    id integer PRIMARY KEY,
    model_id integer REFERENCES models (id),
    no text,
    state integer,
    description text
);

CREATE TABLE batchs (
    id integer PRIMARY KEY,
    state_id integer REFERENCES states (id),
    no text,
    air_no_range text,
    engine_no_range text,
    state integer,
    description text
);

CREATE TABLE airs (
    id integer PRIMARY KEY,
    batch_id integer REFERENCES batchs (id),
    tag text,
    no text,
    factory_id integer REFERENCES organizations (id),
    use_organization_id integer REFERENCES organizations (id),
    offer_time text,
    offer_no text,
    lifespan real,
    residue_span real,
    engine_model text,
    engine_no text,
    engine_lifespan real,
    engine_residue_span real,
    state integer, -- 删除
    description text
);

CREATE TABLE histories (
    id integer PRIMARY KEY,
    baseline_id integer REFERENCES baseline (id),
    name text,
    no text,
    object text,
    abstract text,
    operator text,
    "time" text, -- timestamp
    control_file_id integer, -- REFERENCES "技术状态文件" (id),
    description text
);
