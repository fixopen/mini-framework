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

-- type of type
-- type of info_config
-- is_hidden of schema
-- object_type of baseline

-- type of organization
-- sex of user
-- type of model

(NULL, 65, NULL, 'organizations', 3, '所有的单位'),
(NULL, 66, NULL, 'users', 3, '所有的用户'),
(NULL, 67, NULL, 'models', 3, '所有的机型'),
(NULL, 68, NULL, 'states', 3, '所有的状态'),
(NULL, 69, NULL, 'batchs', 3, '所有的批次'),
(NULL, 70, NULL, 'airs', 3, '所有的飞机'),

(NULL, 129, NULL, 'organizations项类型', 2, '单位的属性'),
(NULL, 130, NULL, 'users项类型', 2, '用户的属性'),
(NULL, 131, NULL, 'models项类型', 2, '机型的属性'),
(NULL, 132, NULL, 'states项类型', 2, '状态的属性'),
(NULL, 133, NULL, 'batchs项类型', 2, '批次的属性'),
(NULL, 134, NULL, 'airs项类型', 2, '飞机的属性'),

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
    restriction_id integer REFERENCES restrictions (id),
    unit_id integer REFERENCES units (id)
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
(NULL, NULL, 129, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 129, 1, 'no', '编号', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 129, 2, 'name', '名称', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 129, 3, 'type', '类型', 1, 262, 0, NULL, NULL, NULL),
(NULL, NULL, 129, 4, 'address', '地址', 1, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 129, 5, 'telephone', '电话', 1, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 129, 6, 'contact', '联系人', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 129, 7, 'manager_id', '管理员id', 1, 66, 0, NULL, NULL, '{"value": "id", "show": "姓名", "filter": "\"组织id\" = id"}'),
(NULL, NULL, 129, 8, 'description', '备注', 1, 2, 0, NULL, NULL, NULL),

(NULL, NULL, 130, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 130, 1, 'login_name', '登录名', 1, 1, 1, NULL, NULL, NULL),
(NULL, NULL, 130, 2, 'name', '姓名', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 130, 3, 'sex', '性别', 1, 263, 0, NULL, NULL, NULL),
(NULL, NULL, 130, 4, 'birthday', '生日', 1, 6, 0, NULL, NULL, NULL),
(NULL, NULL, 130, 5, 'organization_id', '组织id', 1, 65, 0, NULL, NULL, '{"value": "id", "show": "名称"}'),
(NULL, NULL, 130, 6, 'address', '住址', 1, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 130, 7, 'telephone', '电话', 1, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 130, 8, 'email', '电子邮箱', 1, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 130, 9, 'website', '网站', 1, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 130, 10, 'password', '密码', 1, 1, 1, NULL, NULL, NULL),
(NULL, NULL, 130, 11, 'role', '角色', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 130, 12, 'description', '备注', 0, 2, 0, NULL, NULL, NULL),

(NULL, NULL, 131, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 131, 1, 'template_id', '模板id', 1, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 131, 2, 'type', '类型', 0, 264, 0, NULL, NULL, NULL),
(NULL, NULL, 131, 3, 'name', '名称', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 131, 4, 'factory_id', '工厂id', 0, 65, 0, NULL, NULL, '{"value": "id", "show": "名称", "filter": "\"类型\" = 1"}'),
(NULL, NULL, 131, 5, 'state', '状态', 1, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 131, 6, 'description', '备注', 0, 2, 0, NULL, NULL, NULL),

(NULL, NULL, 132, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 132, 1, 'model_id', '机型id', 1, 67, 0, NULL, NULL, '{"value": "id", "show": "名称"}'),
(NULL, NULL, 132, 2, 'no', '编号', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 132, 3, 'state', '状态', 1, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 132, 4, 'description', '备注', 0, 2, 0, NULL, NULL, NULL),

(NULL, NULL, 133, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 133, 1, 'state_id', '状态id', 1, 68, 0, NULL, NULL, '{"value": "id", "show": "编号"}'),
(NULL, NULL, 133, 2, 'no', '编号', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 133, 3, 'air_no_range', '飞机编号范围', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 133, 4, 'engine_no_range', '发动机编号范围', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 133, 5, 'state', '状态', 1, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 133, 6, 'description', '备注', 0, 2, 0, NULL, NULL, NULL),

(NULL, NULL, 134, 0, 'id', 'id', 1, 4, 1, NULL, NULL, '主键'),
(NULL, NULL, 134, 1, 'batch_id', '批次id', 1, 69, 0, NULL, NULL, '{"value": "id", "show": "编号"}'),
(NULL, NULL, 134, 2, 'tag', 'tag', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 134, 3, 'no', '编号', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 134, 4, 'factory_id', '承制单位id', 0, 65, 0, NULL, NULL, '{"value": "id", "show": "名称", "filter": "\"类型\" = 1"}'),
(NULL, NULL, 134, 5, 'user_id', '接装单位id', 1, 65, 0, NULL, NULL, '{"value": "id", "show": "名称", "filter": "\"类型\" = 2"}'),
(NULL, NULL, 134, 6, 'offer_time', '移交时间', 0, 6, 0, NULL, NULL, NULL),
(NULL, NULL, 134, 7, 'offer_no', '移交编号', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 134, 8, 'lifespan', '使用寿命', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 134, 9, 'residue_span', '剩余寿命', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 134, 10, 'engine_model', '引擎型号', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 134, 11, 'engine_no', '引擎编号', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 134, 12, 'engine_lifespan', '引擎使用寿命', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 134, 13, 'engine_residue_span', '引擎剩余寿命', 0, 1, 0, NULL, NULL, NULL),
(NULL, NULL, 134, 14, 'state', '状态', 1, 4, 0, NULL, NULL, NULL),
(NULL, NULL, 134, 15, 'description', '备注', 0, 2, 0, NULL, NULL, NULL);

CREATE TABLE lists (
    id integer PRIMARY KEY,
    template_id integer REFERENCES templates (id),
    parent_no integer, -- REFERENCES types (no),
    element_type_no integer, -- REFERENCES types (no),
    min integer, -- 0
    max integer, -- -1无穷 if (最少==最多) then 列表定长
    group_by text, -- column-name
    description text
);

INSERT INTO lists (id, template_id, parent_no, element_type_no, min, max, group_by, description) VALUES
(NULL, NULL, 65, 129, 0, -1, 'type', NULL),
(NULL, NULL, 66, 130, 0, -1, NULL, NULL),
(NULL, NULL, 67, 131, 0, -1, NULL, NULL),
(NULL, NULL, 68, 132, 0, -1, NULL, NULL),
(NULL, NULL, 69, 133, 0, -1, NULL, NULL),
(NULL, NULL, 70, 134, 0, -1, 'tag', NULL);

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
