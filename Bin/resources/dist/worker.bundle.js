(function (global, factory) {
  typeof exports === 'object' && typeof module !== 'undefined' ? factory(require('@lib/md5'), require('@lib/util')) :
  typeof define === 'function' && define.amd ? define(['@lib/md5', '@lib/util'], factory) :
  (global = typeof globalThis !== 'undefined' ? globalThis : global || self, factory(global.md5));
}(this, (function (md5) { 'use strict';

  function _interopDefaultLegacy (e) { return e && typeof e === 'object' && 'default' in e ? e : { 'default': e }; }

  var md5__default = /*#__PURE__*/_interopDefaultLegacy(md5);

  // import { XLSX } from '../../node_modules/xlsx/dist/xlsx.full.min.js'

  const templates = [
    {
      "id": 1,
      "name": "基础模板",
      "state": 0,
      "description": "基本类型设计"
    }
  ];

  const types = [
    {
        "description": "单行文本",
        "id": 1,
        "name": "文本",
        "no": 1,
        "template_id": null,
        "type": 0
    },
    {
        "description": "可换行文本",
        "id": 2,
        "name": "长文本",
        "no": 2,
        "template_id": null,
        "type": 0
    },
    {
        "description": "整数或者实数、可以规定小数位数",
        "id": 3,
        "name": "数字",
        "no": 3,
        "template_id": null,
        "type": 0
    },
    {
        "description": "整数",
        "id": 4,
        "name": "整数",
        "no": 4,
        "template_id": null,
        "type": 0
    },
    {
        "description": "实数",
        "id": 5,
        "name": "实数",
        "no": 5,
        "template_id": null,
        "type": 0
    },
    {
        "description": "年月日",
        "id": 6,
        "name": "日期",
        "no": 6,
        "template_id": null,
        "type": 0
    },
    {
        "description": "时分秒",
        "id": 7,
        "name": "时间",
        "no": 7,
        "template_id": null,
        "type": 0
    },
    {
        "description": "二进制内容",
        "id": 8,
        "name": "数据对象",
        "no": 8,
        "template_id": null,
        "type": 0
    },
    {
        "description": "图片内容",
        "id": 9,
        "name": "图片",
        "no": 9,
        "template_id": null,
        "type": 0
    },
    {
        "description": "所有的单位",
        "id": 10,
        "name": "组织",
        "no": 65,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有的用户",
        "id": 11,
        "name": "用户",
        "no": 66,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有的机型",
        "id": 12,
        "name": "机型",
        "no": 67,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有的状态",
        "id": 13,
        "name": "状态",
        "no": 68,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有的批次",
        "id": 14,
        "name": "批次",
        "no": 69,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有的基线",
        "id": 15,
        "name": "基线",
        "no": 70,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有的飞机",
        "id": 16,
        "name": "飞机",
        "no": 71,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有的构型",
        "id": 17,
        "name": "构型",
        "no": 72,
        "template_id": null,
        "type": 3
    },
    {
        "description": "四随内容及对应表",
        "id": 18,
        "name": "support_configs",
        "no": 73,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有的随机设备工具",
        "id": 19,
        "name": "随机设备工具",
        "no": 74,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有的机械专业随机备件目录",
        "id": 20,
        "name": "机械专业随机备件",
        "no": 75,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有的随机资料目录",
        "id": 21,
        "name": "随机资料目录",
        "no": 76,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有的随机资料配套目录",
        "id": 22,
        "name": "随机资料配套目录",
        "no": 77,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有的技术状态文件",
        "id": 23,
        "name": "技术状态文件",
        "no": 78,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有的有寿件",
        "id": 24,
        "name": "有寿件",
        "no": 79,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有的双流水",
        "id": 25,
        "name": "双流水",
        "no": 80,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有的交付遗留问题及处置情况",
        "id": 26,
        "name": "交付遗留问题及处置情况",
        "no": 81,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有的变更",
        "id": 27,
        "name": "changed_histories",
        "no": 82,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有的变更详情",
        "id": 28,
        "name": "changed_history_details",
        "no": 83,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有的二进制文件",
        "id": 29,
        "name": "files",
        "no": 84,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有信息的配置表",
        "id": 30,
        "name": "info_configs",
        "no": 85,
        "template_id": null,
        "type": 3
    },
    {
        "description": "所有软件",
        "id": 31,
        "name": "softwares",
        "no": 86,
        "template_id": null,
        "type": 3
    },
    {
        "description": "单位的属性",
        "id": 32,
        "name": "组织表类型",
        "no": 129,
        "template_id": null,
        "type": 2
    },
    {
        "description": "用户的属性",
        "id": 33,
        "name": "用户表类型",
        "no": 130,
        "template_id": null,
        "type": 2
    },
    {
        "description": "机型的属性",
        "id": 34,
        "name": "机型表类型",
        "no": 131,
        "template_id": null,
        "type": 2
    },
    {
        "description": "状态的属性",
        "id": 35,
        "name": "状态表类型",
        "no": 132,
        "template_id": null,
        "type": 2
    },
    {
        "description": "批次的属性",
        "id": 36,
        "name": "批次表类型",
        "no": 133,
        "template_id": null,
        "type": 2
    },
    {
        "description": "基线的属性",
        "id": 37,
        "name": "基线表类型",
        "no": 134,
        "template_id": null,
        "type": 2
    },
    {
        "description": "飞机的属性",
        "id": 38,
        "name": "飞机表类型",
        "no": 135,
        "template_id": null,
        "type": 2
    },
    {
        "description": "构型的属性",
        "id": 39,
        "name": "构型表类型",
        "no": 136,
        "template_id": null,
        "type": 2
    },
    {
        "description": "四随内容及对应项",
        "id": 40,
        "name": "support_configs表类型",
        "no": 137,
        "template_id": null,
        "type": 2
    },
    {
        "description": "随机设备工具的属性",
        "id": 41,
        "name": "随机设备工具表类型",
        "no": 138,
        "template_id": null,
        "type": 2
    },
    {
        "description": "机械专业随机备件目录的属性",
        "id": 42,
        "name": "机械专业随机备件目录表类型",
        "no": 139,
        "template_id": null,
        "type": 2
    },
    {
        "description": "随机资料目录的属性",
        "id": 43,
        "name": "随机资料目录表类型",
        "no": 140,
        "template_id": null,
        "type": 2
    },
    {
        "description": "随机资料配套目录的属性",
        "id": 44,
        "name": "随机资料配套目录表类型",
        "no": 141,
        "template_id": null,
        "type": 2
    },
    {
        "description": "技术状态文件的属性",
        "id": 45,
        "name": "技术状态文件表类型",
        "no": 142,
        "template_id": null,
        "type": 2
    },
    {
        "description": "有寿件的属性",
        "id": 46,
        "name": "有寿件表类型",
        "no": 143,
        "template_id": null,
        "type": 2
    },
    {
        "description": "双流水的属性",
        "id": 47,
        "name": "双流水表类型",
        "no": 144,
        "template_id": null,
        "type": 2
    },
    {
        "description": "交付遗留问题及处置情况的属性",
        "id": 48,
        "name": "交付遗留问题及处置情况表类型",
        "no": 145,
        "template_id": null,
        "type": 2
    },
    {
        "description": "变更的属性",
        "id": 49,
        "name": "changed_histories表类型",
        "no": 146,
        "template_id": null,
        "type": 2
    },
    {
        "description": "变更详情的属性",
        "id": 50,
        "name": "changed_history_details表类型",
        "no": 147,
        "template_id": null,
        "type": 2
    },
    {
        "description": "文件的基本属性",
        "id": 51,
        "name": "files表类型",
        "no": 148,
        "template_id": null,
        "type": 2
    },
    {
        "description": "信息的配置项",
        "id": 52,
        "name": "info_configs表类型",
        "no": 149,
        "template_id": null,
        "type": 3
    },
    {
        "description": "软件的基本属性",
        "id": 53,
        "name": "softwares表类型",
        "no": 150,
        "template_id": null,
        "type": 2
    },
    {
        "description": "三种组织的类型",
        "id": 54,
        "name": "组织类型",
        "no": 257,
        "template_id": 1,
        "type": 1
    },
    {
        "description": "表示性别的类型",
        "id": 55,
        "name": "性别类型",
        "no": 258,
        "template_id": 1,
        "type": 1
    },
    {
        "description": "表示飞机的类型",
        "id": 56,
        "name": "飞机类型",
        "no": 259,
        "template_id": 1,
        "type": 1
    },
    {
        "description": "用于机型扩展字段的类型定义",
        "id": 57,
        "name": "机型扩展类型",
        "no": 260,
        "template_id": 1,
        "type": 2
    },
    {
        "description": "用于状态扩展字段的类型定义",
        "id": 58,
        "name": "状态扩展类型",
        "no": 261,
        "template_id": 1,
        "type": 2
    },
    {
        "description": "用于布局的类型定义",
        "id": 59,
        "name": "布局类型",
        "no": 262,
        "template_id": 1,
        "type": 2
    },
    {
        "description": "用于技术状态的类型定义",
        "id": 60,
        "name": "技术状态类型",
        "no": 263,
        "template_id": 1,
        "type": 2
    },
    {
        "description": "用于主要配备的类型定义",
        "id": 61,
        "name": "主要配备类型",
        "no": 264,
        "template_id": 1,
        "type": 2
    },
    {
        "description": "用于批次扩展字段的类型定义",
        "id": 62,
        "name": "批次扩展类型",
        "no": 265,
        "template_id": 1,
        "type": 2
    },
    {
        "description": "批次或单机",
        "id": 63,
        "name": "基线适用目标类型",
        "no": 266,
        "template_id": null,
        "type": 1
    },
    {
        "description": "用于基线扩展字段的类型定义",
        "id": 64,
        "name": "基线扩展类型",
        "no": 267,
        "template_id": 1,
        "type": 2
    },
    {
        "description": "用于飞机扩展字段的类型定义",
        "id": 65,
        "name": "飞机扩展类型",
        "no": 268,
        "template_id": 1,
        "type": 2
    },
    {
        "description": "随机设备工具隶属的专业",
        "id": 66,
        "name": "专业类型",
        "no": 269,
        "template_id": 1,
        "type": 1
    },
    {
        "description": null,
        "id": 67,
        "name": "配套比例类型",
        "no": 270,
        "template_id": 1,
        "type": 2
    },
    {
        "description": "用于随机设备工具扩展字段的类型定义",
        "id": 68,
        "name": "随机设备工具扩展类型",
        "no": 271,
        "template_id": 1,
        "type": 2
    },
    {
        "description": null,
        "id": 69,
        "name": "比例和数量类型",
        "no": 272,
        "template_id": 1,
        "type": 2
    },
    {
        "description": "用于机械专业随机备件扩展字段的类型定义",
        "id": 70,
        "name": "机械专业随机备件扩展类型",
        "no": 273,
        "template_id": 1,
        "type": 2
    },
    {
        "description": "用于随机资料目录扩展字段的类型定义",
        "id": 71,
        "name": "随机资料目录扩展类型",
        "no": 274,
        "template_id": 1,
        "type": 2
    },
    {
        "description": "用于随机资料配套目录扩展字段的类型定义",
        "id": 72,
        "name": "随机资料配套目录扩展类型",
        "no": 275,
        "template_id": 1,
        "type": 2
    },
    {
        "description": "20种技术状态文件类型",
        "id": 73,
        "name": "技术状态文件类型",
        "no": 276,
        "template_id": 1,
        "type": 1
    },
    {
        "description": "用于技术状态文件扩展字段的类型定义",
        "id": 74,
        "name": "技术状态文件扩展类型",
        "no": 277,
        "template_id": 1,
        "type": 2
    },
    {
        "description": "用于有寿件扩展字段的类型定义",
        "id": 75,
        "name": "有寿件扩展类型",
        "no": 278,
        "template_id": 1,
        "type": 2
    },
    {
        "description": "用于双流水扩展字段的类型定义",
        "id": 76,
        "name": "双流水扩展类型",
        "no": 279,
        "template_id": 1,
        "type": 2
    },
    {
        "description": "用于交付遗留问题及处置情况扩展字段的类型定义",
        "id": 77,
        "name": "交付遗留问题及处置情况扩展类型",
        "no": 280,
        "template_id": 1,
        "type": 2
    },
    {
        "description": "用于software扩展字段的类型定义",
        "id": 78,
        "name": "software扩展类型",
        "no": 281,
        "template_id": 1,
        "type": 2
    },
    {
        "description": null,
        "id": 79,
        "name": "五向图类型",
        "no": 1025,
        "template_id": 1,
        "type": 3
    },
    {
        "description": null,
        "id": 80,
        "name": "外形参数类型",
        "no": 1026,
        "template_id": 1,
        "type": 2
    },
    {
        "description": null,
        "id": 81,
        "name": "飞行性能类型",
        "no": 1027,
        "template_id": 1,
        "type": 2
    },
    {
        "description": null,
        "id": 82,
        "name": "寿命类型",
        "no": 1028,
        "template_id": 1,
        "type": 2
    },
    {
        "description": null,
        "id": 83,
        "name": "武器类型",
        "no": 1029,
        "template_id": 1,
        "type": 2
    },
    {
        "description": null,
        "id": 84,
        "name": "动力装置类型",
        "no": 1030,
        "template_id": 1,
        "type": 2
    },
    {
        "description": null,
        "id": 85,
        "name": "雷达配置类型",
        "no": 1031,
        "template_id": 1,
        "type": 2
    },
    {
        "description": null,
        "id": 86,
        "name": "电子对抗类型",
        "no": 1032,
        "template_id": 1,
        "type": 2
    },
    {
        "description": null,
        "id": 87,
        "name": "外挂吊仓类型",
        "no": 1033,
        "template_id": 1,
        "type": 2
    },
    {
        "description": null,
        "id": 88,
        "name": "数据链类型",
        "no": 1034,
        "template_id": 1,
        "type": 1
    },
    {
        "description": null,
        "id": 89,
        "name": "座椅类型",
        "no": 1035,
        "template_id": 1,
        "type": 2
    },
    {
        "description": null,
        "id": 90,
        "name": "武器外挂类型",
        "no": 1036,
        "template_id": 1,
        "type": 3
    },
    {
        "description": null,
        "id": 91,
        "name": "挂梁挂柱类型",
        "no": 1037,
        "template_id": 1,
        "type": 3
    },
    {
        "description": null,
        "id": 92,
        "name": "武器外挂表类型",
        "no": 1038,
        "template_id": 1,
        "type": 2
    },
    {
        "description": null,
        "id": 93,
        "name": "挂梁挂柱表类型",
        "no": 1039,
        "template_id": 1,
        "type": 2
    },
    {
        "description": null,
        "id": 94,
        "name": "图片文件类型",
        "no": 1040,
        "template_id": 1,
        "type": 2
    }
  ];

  const codes = [
    {
        "description": "用于设计数据类型并分析数据差异的单位",
        "id": 1,
        "no": 0,
        "parent_no": 257,
        "template_id": 1,
        "value": "管理单位"
    },
    {
        "description": "用于填报数据的单位",
        "id": 2,
        "no": 1,
        "parent_no": 257,
        "template_id": 1,
        "value": "承制单位"
    },
    {
        "description": "用于维护单机技术状态并进行状态变更的单位",
        "id": 3,
        "no": 2,
        "parent_no": 257,
        "template_id": 1,
        "value": "接装单位"
    },
    {
        "description": "女",
        "id": 4,
        "no": 0,
        "parent_no": 258,
        "template_id": 1,
        "value": "女"
    },
    {
        "description": "男",
        "id": 5,
        "no": 1,
        "parent_no": 258,
        "template_id": 1,
        "value": "男"
    },
    {
        "description": null,
        "id": 6,
        "no": 0,
        "parent_no": 259,
        "template_id": 1,
        "value": "歼击机"
    },
    {
        "description": null,
        "id": 7,
        "no": 1,
        "parent_no": 259,
        "template_id": 1,
        "value": "强击机"
    },
    {
        "description": null,
        "id": 8,
        "no": 2,
        "parent_no": 259,
        "template_id": 1,
        "value": "轰炸机"
    },
    {
        "description": null,
        "id": 9,
        "no": 3,
        "parent_no": 259,
        "template_id": 1,
        "value": "运输机"
    },
    {
        "description": null,
        "id": 10,
        "no": 5,
        "parent_no": 259,
        "template_id": 1,
        "value": "直升机"
    },
    {
        "description": null,
        "id": 11,
        "no": 6,
        "parent_no": 259,
        "template_id": 1,
        "value": "教练机"
    },
    {
        "description": null,
        "id": 12,
        "no": 7,
        "parent_no": 259,
        "template_id": 1,
        "value": "特种机"
    },
    {
        "description": null,
        "id": 13,
        "no": 8,
        "parent_no": 259,
        "template_id": 1,
        "value": "无人机"
    },
    {
        "description": null,
        "id": 14,
        "no": 0,
        "parent_no": 266,
        "template_id": null,
        "value": "批次"
    },
    {
        "description": null,
        "id": 15,
        "no": 1,
        "parent_no": 266,
        "template_id": null,
        "value": "单机"
    },
    {
        "description": null,
        "id": 16,
        "no": 0,
        "parent_no": 269,
        "template_id": 1,
        "value": "机械专业"
    },
    {
        "description": null,
        "id": 17,
        "no": 1,
        "parent_no": 269,
        "template_id": 1,
        "value": "军械专业"
    },
    {
        "description": null,
        "id": 18,
        "no": 2,
        "parent_no": 269,
        "template_id": 1,
        "value": "特设专业"
    },
    {
        "description": null,
        "id": 19,
        "no": 3,
        "parent_no": 269,
        "template_id": 1,
        "value": "航电专业"
    },
    {
        "description": null,
        "id": 20,
        "no": 4,
        "parent_no": 269,
        "template_id": 1,
        "value": "综合航电专业"
    },
    {
        "description": null,
        "id": 21,
        "no": 5,
        "parent_no": 269,
        "template_id": 1,
        "value": "通用工具"
    },
    {
        "description": null,
        "id": 22,
        "no": 6,
        "parent_no": 269,
        "template_id": 1,
        "value": "电气专业"
    },
    {
        "description": null,
        "id": 23,
        "no": 7,
        "parent_no": 269,
        "template_id": 1,
        "value": "飞控专业"
    },
    {
        "description": null,
        "id": 24,
        "no": 8,
        "parent_no": 269,
        "template_id": 1,
        "value": "起落架专业"
    },
    {
        "description": "files",
        "id": 25,
        "no": 0,
        "parent_no": 276,
        "template_id": 1,
        "value": "机械设备产品规范、材料规范、工艺规范、软件规范清单"
    },
    {
        "description": "file",
        "id": 26,
        "no": 1,
        "parent_no": 276,
        "template_id": 1,
        "value": "机械设备产品规范树"
    },
    {
        "description": "tree",
        "id": 27,
        "no": 2,
        "parent_no": 276,
        "template_id": 1,
        "value": "产品规范目录"
    },
    {
        "description": "tree",
        "id": 28,
        "no": 3,
        "parent_no": 276,
        "template_id": 1,
        "value": "材料文件目录"
    },
    {
        "description": "tree",
        "id": 29,
        "no": 4,
        "parent_no": 276,
        "template_id": 1,
        "value": "工艺文件目录"
    },
    {
        "description": "tree",
        "id": 30,
        "no": 5,
        "parent_no": 276,
        "template_id": 1,
        "value": "软件开发文档目录"
    },
    {
        "description": "files",
        "id": 31,
        "no": 6,
        "parent_no": 276,
        "template_id": 1,
        "value": "机械设备整机图样目录"
    },
    {
        "description": "tree",
        "id": 32,
        "no": 7,
        "parent_no": 276,
        "template_id": 1,
        "value": "用户技术资料目录"
    },
    {
        "description": "file",
        "id": 33,
        "no": 8,
        "parent_no": 276,
        "template_id": 1,
        "value": "自制件汇总表"
    },
    {
        "description": "file",
        "id": 34,
        "no": 9,
        "parent_no": 276,
        "template_id": 1,
        "value": "标准件汇总表"
    },
    {
        "description": "file",
        "id": 35,
        "no": 10,
        "parent_no": 276,
        "template_id": 1,
        "value": "外购件汇总表"
    },
    {
        "description": "files",
        "id": 36,
        "no": 11,
        "parent_no": 276,
        "template_id": 1,
        "value": "经批准的技术状态文件更改申请单目录"
    },
    {
        "description": "files",
        "id": 37,
        "no": 12,
        "parent_no": 276,
        "template_id": 1,
        "value": "经批准的偏离许可、让步申请单目录"
    },
    {
        "description": "file",
        "id": 38,
        "no": 13,
        "parent_no": 276,
        "template_id": 1,
        "value": "装备配套表"
    },
    {
        "description": "file",
        "id": 39,
        "no": 14,
        "parent_no": 276,
        "template_id": 1,
        "value": "软件配置项汇总表"
    },
    {
        "description": "files",
        "id": 40,
        "no": 15,
        "parent_no": 276,
        "template_id": 1,
        "value": "随机配套设备目录"
    },
    {
        "description": "files",
        "id": 41,
        "no": 16,
        "parent_no": 276,
        "template_id": 1,
        "value": "随机配套工具目录"
    },
    {
        "description": "tree",
        "id": 42,
        "no": 17,
        "parent_no": 276,
        "template_id": 1,
        "value": "随机配套资料目录"
    },
    {
        "description": "files",
        "id": 43,
        "no": 18,
        "parent_no": 276,
        "template_id": 1,
        "value": "随机配套备件目录"
    },
    {
        "description": "files",
        "id": 44,
        "no": 19,
        "parent_no": 276,
        "template_id": 1,
        "value": "履历本/合格证目录"
    },
    {
        "description": null,
        "id": 45,
        "no": 0,
        "parent_no": 1034,
        "template_id": 1,
        "value": "J"
    },
    {
        "description": null,
        "id": 46,
        "no": 1,
        "parent_no": 1034,
        "template_id": 1,
        "value": "U"
    },
    {
        "description": null,
        "id": 47,
        "no": 2,
        "parent_no": 1034,
        "template_id": 1,
        "value": "V"
    }
  ];

  const schemas = [
    {
        "description": "主键",
        "id": 1,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 129,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "单位的编号",
        "id": 2,
        "is_required": 0,
        "name": "编号",
        "no": 1,
        "parent_no": 129,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "单位的名称",
        "id": 3,
        "is_required": 0,
        "name": "名称",
        "no": 2,
        "parent_no": 129,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "单位的类型",
        "id": 4,
        "is_required": 0,
        "name": "类型",
        "no": 3,
        "parent_no": 129,
        "restriction": null,
        "template_id": null,
        "type_no": 257
    },
    {
        "description": "单位的地址",
        "id": 5,
        "is_required": 0,
        "name": "地址",
        "no": 4,
        "parent_no": 129,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "单位联系电话",
        "id": 6,
        "is_required": 0,
        "name": "电话",
        "no": 5,
        "parent_no": 129,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "单位联系人",
        "id": 7,
        "is_required": 0,
        "name": "联系人",
        "no": 6,
        "parent_no": 129,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"姓名\", \"filter\": \"\\\"组织id\\\" = id\"}",
        "id": 8,
        "is_required": 0,
        "name": "管理员id",
        "no": 7,
        "parent_no": 129,
        "restriction": null,
        "template_id": null,
        "type_no": 66
    },
    {
        "description": null,
        "id": 9,
        "is_required": 0,
        "name": "备注",
        "no": 8,
        "parent_no": 129,
        "restriction": null,
        "template_id": null,
        "type_no": 2
    },
    {
        "description": "主键",
        "id": 10,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 130,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "登录名",
        "id": 11,
        "is_required": 1,
        "name": "登录名",
        "no": 1,
        "parent_no": 130,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "显示姓名",
        "id": 12,
        "is_required": 0,
        "name": "姓名",
        "no": 2,
        "parent_no": 130,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "男女",
        "id": 13,
        "is_required": 0,
        "name": "性别",
        "no": 3,
        "parent_no": 130,
        "restriction": null,
        "template_id": null,
        "type_no": 258
    },
    {
        "description": "2006-01-02",
        "id": 14,
        "is_required": 0,
        "name": "生日",
        "no": 4,
        "parent_no": 130,
        "restriction": null,
        "template_id": null,
        "type_no": 6
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 15,
        "is_required": 0,
        "name": "组织id",
        "no": 5,
        "parent_no": 130,
        "restriction": null,
        "template_id": null,
        "type_no": 65
    },
    {
        "description": "居住地",
        "id": 16,
        "is_required": 0,
        "name": "住址",
        "no": 6,
        "parent_no": 130,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "联系电话",
        "id": 17,
        "is_required": 0,
        "name": "电话",
        "no": 7,
        "parent_no": 130,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "email地址",
        "id": 18,
        "is_required": 0,
        "name": "email",
        "no": 8,
        "parent_no": 130,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "个人主页",
        "id": 19,
        "is_required": 0,
        "name": "website",
        "no": 9,
        "parent_no": 130,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "登录密码",
        "id": 20,
        "is_required": 1,
        "name": "密码",
        "no": 10,
        "parent_no": 130,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "角色",
        "id": 21,
        "is_required": 0,
        "name": "role",
        "no": 11,
        "parent_no": 130,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 22,
        "is_required": 0,
        "name": "备注",
        "no": 12,
        "parent_no": 130,
        "restriction": null,
        "template_id": null,
        "type_no": 2
    },
    {
        "description": "主键",
        "id": 23,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 131,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "采用的类型定义模板的id",
        "id": 24,
        "is_required": 0,
        "name": "模板id",
        "no": 1,
        "parent_no": 131,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "属于哪类飞机",
        "id": 25,
        "is_required": 0,
        "name": "类型",
        "no": 2,
        "parent_no": 131,
        "restriction": null,
        "template_id": null,
        "type_no": 259
    },
    {
        "description": "飞机模型的名称",
        "id": 26,
        "is_required": 0,
        "name": "名称",
        "no": 3,
        "parent_no": 131,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\", \"filter\": \"\\\"类型\\\" = 1\"}",
        "id": 27,
        "is_required": 0,
        "name": "工厂id",
        "no": 4,
        "parent_no": 131,
        "restriction": null,
        "template_id": null,
        "type_no": 65
    },
    {
        "description": null,
        "id": 28,
        "is_required": 0,
        "name": "state",
        "no": 5,
        "parent_no": 131,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "机型的扩展数据",
        "id": 29,
        "is_required": 0,
        "name": "扩展",
        "no": 6,
        "parent_no": 131,
        "restriction": null,
        "template_id": null,
        "type_no": 260
    },
    {
        "description": null,
        "id": 30,
        "is_required": 0,
        "name": "备注",
        "no": 7,
        "parent_no": 131,
        "restriction": null,
        "template_id": null,
        "type_no": 2
    },
    {
        "description": "主键",
        "id": 31,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 132,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 32,
        "is_required": 0,
        "name": "机型id",
        "no": 1,
        "parent_no": 132,
        "restriction": null,
        "template_id": null,
        "type_no": 67
    },
    {
        "description": "机型状态的编号",
        "id": 33,
        "is_required": 0,
        "name": "编号",
        "no": 2,
        "parent_no": 132,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 34,
        "is_required": 0,
        "name": "使命任务",
        "no": 3,
        "parent_no": 132,
        "restriction": null,
        "template_id": null,
        "type_no": 2
    },
    {
        "description": null,
        "id": 35,
        "is_required": 0,
        "name": "state",
        "no": 4,
        "parent_no": 132,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "机型状态的扩展数据",
        "id": 36,
        "is_required": 0,
        "name": "扩展",
        "no": 5,
        "parent_no": 132,
        "restriction": null,
        "template_id": null,
        "type_no": 261
    },
    {
        "description": null,
        "id": 37,
        "is_required": 0,
        "name": "备注",
        "no": 6,
        "parent_no": 132,
        "restriction": null,
        "template_id": null,
        "type_no": 2
    },
    {
        "description": "主键",
        "id": 38,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 133,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"编号\"}",
        "id": 39,
        "is_required": 0,
        "name": "状态id",
        "no": 1,
        "parent_no": 133,
        "restriction": null,
        "template_id": null,
        "type_no": 68
    },
    {
        "description": "批次的编号",
        "id": 40,
        "is_required": 0,
        "name": "编号",
        "no": 2,
        "parent_no": 133,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 41,
        "is_required": 0,
        "name": "飞机编号范围",
        "no": 3,
        "parent_no": 133,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 42,
        "is_required": 0,
        "name": "发动机编号范围",
        "no": 4,
        "parent_no": 133,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 43,
        "is_required": 0,
        "name": "布局",
        "no": 5,
        "parent_no": 133,
        "restriction": null,
        "template_id": null,
        "type_no": 262
    },
    {
        "description": null,
        "id": 44,
        "is_required": 0,
        "name": "主要技术参数",
        "no": 6,
        "parent_no": 133,
        "restriction": null,
        "template_id": null,
        "type_no": 263
    },
    {
        "description": null,
        "id": 45,
        "is_required": 0,
        "name": "主要配备",
        "no": 7,
        "parent_no": 133,
        "restriction": null,
        "template_id": null,
        "type_no": 264
    },
    {
        "description": null,
        "id": 46,
        "is_required": 0,
        "name": "喷涂方案",
        "no": 8,
        "parent_no": 133,
        "restriction": null,
        "template_id": null,
        "type_no": 8
    },
    {
        "description": null,
        "id": 47,
        "is_required": 0,
        "name": "state",
        "no": 9,
        "parent_no": 133,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "批次的扩展数据",
        "id": 48,
        "is_required": 0,
        "name": "扩展",
        "no": 10,
        "parent_no": 133,
        "restriction": null,
        "template_id": null,
        "type_no": 265
    },
    {
        "description": null,
        "id": 49,
        "is_required": 0,
        "name": "备注",
        "no": 11,
        "parent_no": 133,
        "restriction": null,
        "template_id": null,
        "type_no": 2
    },
    {
        "description": "主键",
        "id": 50,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 134,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": null,
        "id": 51,
        "is_required": 0,
        "name": "适用目标",
        "no": 1,
        "parent_no": 134,
        "restriction": null,
        "template_id": null,
        "type_no": 266
    },
    {
        "description": "批次或单机的id",
        "id": 52,
        "is_required": 0,
        "name": "目标id",
        "no": 2,
        "parent_no": 134,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": null,
        "id": 53,
        "is_required": 0,
        "name": "state",
        "no": 3,
        "parent_no": 134,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "基线的扩展数据",
        "id": 54,
        "is_required": 0,
        "name": "扩展",
        "no": 4,
        "parent_no": 134,
        "restriction": null,
        "template_id": null,
        "type_no": 267
    },
    {
        "description": null,
        "id": 55,
        "is_required": 0,
        "name": "备注",
        "no": 5,
        "parent_no": 134,
        "restriction": null,
        "template_id": null,
        "type_no": 2
    },
    {
        "description": "主键",
        "id": 56,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 135,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"编号\"}",
        "id": 57,
        "is_required": 0,
        "name": "批次id",
        "no": 1,
        "parent_no": 135,
        "restriction": null,
        "template_id": null,
        "type_no": 70
    },
    {
        "description": null,
        "id": 58,
        "is_required": 0,
        "name": "tag",
        "no": 2,
        "parent_no": 135,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "飞机的编号",
        "id": 59,
        "is_required": 0,
        "name": "编号",
        "no": 3,
        "parent_no": 135,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\", \"filter\": \"\\\"类型\\\" = 1\"}",
        "id": 60,
        "is_required": 0,
        "name": "承制单位id",
        "no": 4,
        "parent_no": 135,
        "restriction": null,
        "template_id": null,
        "type_no": 65
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\", \"filter\": \"\\\"类型\\\" = 2\"}",
        "id": 61,
        "is_required": 0,
        "name": "接装单位id",
        "no": 5,
        "parent_no": 135,
        "restriction": null,
        "template_id": null,
        "type_no": 65
    },
    {
        "description": "2006-01-02 15:04:05",
        "id": 62,
        "is_required": 0,
        "name": "移交时间",
        "no": 6,
        "parent_no": 135,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": null,
        "id": 63,
        "is_required": 0,
        "name": "移交编号",
        "no": 7,
        "parent_no": 135,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 64,
        "is_required": 0,
        "name": "使用寿命",
        "no": 8,
        "parent_no": 135,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 65,
        "is_required": 0,
        "name": "剩余寿命",
        "no": 9,
        "parent_no": 135,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 66,
        "is_required": 0,
        "name": "引擎型号",
        "no": 10,
        "parent_no": 135,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 67,
        "is_required": 0,
        "name": "引擎编号",
        "no": 11,
        "parent_no": 135,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 68,
        "is_required": 0,
        "name": "引擎使用寿命",
        "no": 12,
        "parent_no": 135,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 69,
        "is_required": 0,
        "name": "引擎剩余寿命",
        "no": 13,
        "parent_no": 135,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 70,
        "is_required": 0,
        "name": "state",
        "no": 14,
        "parent_no": 135,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "飞机的扩展数据",
        "id": 71,
        "is_required": 0,
        "name": "扩展",
        "no": 15,
        "parent_no": 135,
        "restriction": null,
        "template_id": null,
        "type_no": 268
    },
    {
        "description": null,
        "id": 72,
        "is_required": 0,
        "name": "备注",
        "no": 16,
        "parent_no": 135,
        "restriction": null,
        "template_id": null,
        "type_no": 2
    },
    {
        "description": "主键",
        "id": 73,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 136,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 74,
        "is_required": 0,
        "name": "基线id",
        "no": 1,
        "parent_no": 136,
        "restriction": null,
        "template_id": null,
        "type_no": 70
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 75,
        "is_required": 0,
        "name": "parent_id",
        "no": 2,
        "parent_no": 136,
        "restriction": null,
        "template_id": null,
        "type_no": 72
    },
    {
        "description": null,
        "id": 76,
        "is_required": 0,
        "name": "序号",
        "no": 3,
        "parent_no": 136,
        "restriction": null,
        "template_id": null,
        "type_no": 4
    },
    {
        "description": null,
        "id": 77,
        "is_required": 0,
        "name": "名称",
        "no": 5,
        "parent_no": 136,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 78,
        "is_required": 0,
        "name": "型号",
        "no": 6,
        "parent_no": 136,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\", \"filter\": \"\\\"类型\\\" = 1\"}",
        "id": 79,
        "is_required": 0,
        "name": "承制单位id",
        "no": 9,
        "parent_no": 136,
        "restriction": null,
        "template_id": null,
        "type_no": 65
    },
    {
        "description": "构型的扩展数据，不能定义，每个节点的扩展不同",
        "id": 80,
        "is_required": 0,
        "name": "extended",
        "no": 13,
        "parent_no": 136,
        "restriction": null,
        "template_id": null,
        "type_no": 2
    },
    {
        "description": null,
        "id": 81,
        "is_required": 0,
        "name": "备注",
        "no": 14,
        "parent_no": 136,
        "restriction": null,
        "template_id": null,
        "type_no": 2
    },
    {
        "description": "主键",
        "id": 82,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 137,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": null,
        "id": 83,
        "is_required": 0,
        "name": "template_id",
        "no": 1,
        "parent_no": 137,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": null,
        "id": 84,
        "is_required": 0,
        "name": "parent_id",
        "no": 2,
        "parent_no": 137,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": null,
        "id": 85,
        "is_required": 0,
        "name": "name",
        "no": 3,
        "parent_no": 137,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 86,
        "is_required": 0,
        "name": "table_no",
        "no": 4,
        "parent_no": 137,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "主键",
        "id": 87,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 138,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 88,
        "is_required": 0,
        "name": "基线id",
        "no": 1,
        "parent_no": 138,
        "restriction": null,
        "template_id": null,
        "type_no": 70
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 89,
        "is_required": 0,
        "name": "parent_id",
        "no": 2,
        "parent_no": 138,
        "restriction": null,
        "template_id": null,
        "type_no": 74
    },
    {
        "description": null,
        "id": 90,
        "is_required": 0,
        "name": "专业",
        "no": 3,
        "parent_no": 138,
        "restriction": null,
        "template_id": null,
        "type_no": 269
    },
    {
        "description": null,
        "id": 91,
        "is_required": 0,
        "name": "序号",
        "no": 4,
        "parent_no": 138,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 92,
        "is_required": 0,
        "name": "名称",
        "no": 5,
        "parent_no": 138,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 93,
        "is_required": 0,
        "name": "型号",
        "no": 6,
        "parent_no": 138,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 94,
        "is_required": 0,
        "name": "用途",
        "no": 7,
        "parent_no": 138,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 95,
        "is_required": 0,
        "name": "配套比例",
        "no": 8,
        "parent_no": 138,
        "restriction": null,
        "template_id": null,
        "type_no": 270
    },
    {
        "description": null,
        "id": 96,
        "is_required": 0,
        "name": "定价",
        "no": 9,
        "parent_no": 138,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\", \"filter\": \"\\\"类型\\\" = 1\"}",
        "id": 97,
        "is_required": 0,
        "name": "承制单位id",
        "no": 10,
        "parent_no": 138,
        "restriction": null,
        "template_id": null,
        "type_no": 65
    },
    {
        "description": null,
        "id": 98,
        "is_required": 0,
        "name": "扩展",
        "no": 11,
        "parent_no": 138,
        "restriction": null,
        "template_id": null,
        "type_no": 271
    },
    {
        "description": null,
        "id": 99,
        "is_required": 0,
        "name": "备注",
        "no": 12,
        "parent_no": 138,
        "restriction": null,
        "template_id": null,
        "type_no": 2
    },
    {
        "description": "主键",
        "id": 100,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 139,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 101,
        "is_required": 0,
        "name": "基线id",
        "no": 1,
        "parent_no": 139,
        "restriction": null,
        "template_id": null,
        "type_no": 70
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 102,
        "is_required": 0,
        "name": "parent_id",
        "no": 2,
        "parent_no": 139,
        "restriction": null,
        "template_id": null,
        "type_no": 75
    },
    {
        "description": null,
        "id": 103,
        "is_required": 0,
        "name": "序号",
        "no": 3,
        "parent_no": 139,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 104,
        "is_required": 0,
        "name": "名称",
        "no": 4,
        "parent_no": 139,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 105,
        "is_required": 0,
        "name": "型号(图号)",
        "no": 5,
        "parent_no": 139,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 106,
        "is_required": 0,
        "name": "单机数量",
        "no": 6,
        "parent_no": 139,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 107,
        "is_required": 0,
        "name": "比例及数量",
        "no": 7,
        "parent_no": 139,
        "restriction": null,
        "template_id": null,
        "type_no": 272
    },
    {
        "description": null,
        "id": 108,
        "is_required": 0,
        "name": "所属系统",
        "no": 8,
        "parent_no": 139,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 109,
        "is_required": 0,
        "name": "安装使用部位",
        "no": 9,
        "parent_no": 139,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\", \"filter\": \"\\\"类型\\\" = 1\"}",
        "id": 110,
        "is_required": 0,
        "name": "承制单位id",
        "no": 10,
        "parent_no": 139,
        "restriction": null,
        "template_id": null,
        "type_no": 65
    },
    {
        "description": null,
        "id": 111,
        "is_required": 0,
        "name": "扩展",
        "no": 11,
        "parent_no": 139,
        "restriction": null,
        "template_id": null,
        "type_no": 273
    },
    {
        "description": null,
        "id": 112,
        "is_required": 0,
        "name": "备注",
        "no": 12,
        "parent_no": 139,
        "restriction": null,
        "template_id": null,
        "type_no": 2
    },
    {
        "description": "主键",
        "id": 113,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 140,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 114,
        "is_required": 0,
        "name": "基线id",
        "no": 1,
        "parent_no": 140,
        "restriction": null,
        "template_id": null,
        "type_no": 70
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"序号\"}",
        "id": 115,
        "is_required": 0,
        "name": "parent_id",
        "no": 2,
        "parent_no": 140,
        "restriction": null,
        "template_id": null,
        "type_no": 76
    },
    {
        "description": null,
        "id": 116,
        "is_required": 0,
        "name": "序号",
        "no": 3,
        "parent_no": 140,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 117,
        "is_required": 0,
        "name": "资料编号",
        "no": 4,
        "parent_no": 140,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 118,
        "is_required": 0,
        "name": "手册名称",
        "no": 5,
        "parent_no": 140,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 119,
        "is_required": 0,
        "name": "册数",
        "no": 6,
        "parent_no": 140,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 120,
        "is_required": 0,
        "name": "单册页数",
        "no": 7,
        "parent_no": 140,
        "restriction": null,
        "template_id": null,
        "type_no": 5
    },
    {
        "description": null,
        "id": 121,
        "is_required": 0,
        "name": "密级",
        "no": 8,
        "parent_no": 140,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 122,
        "is_required": 0,
        "name": "扩展",
        "no": 9,
        "parent_no": 140,
        "restriction": null,
        "template_id": null,
        "type_no": 274
    },
    {
        "description": null,
        "id": 123,
        "is_required": 0,
        "name": "备注",
        "no": 10,
        "parent_no": 140,
        "restriction": null,
        "template_id": null,
        "type_no": 2
    },
    {
        "description": "主键",
        "id": 124,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 141,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 125,
        "is_required": 0,
        "name": "基线id",
        "no": 1,
        "parent_no": 141,
        "restriction": null,
        "template_id": null,
        "type_no": 70
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"序号\"}",
        "id": 126,
        "is_required": 0,
        "name": "parent_id",
        "no": 2,
        "parent_no": 141,
        "restriction": null,
        "template_id": null,
        "type_no": 77
    },
    {
        "description": null,
        "id": 127,
        "is_required": 0,
        "name": "序号",
        "no": 3,
        "parent_no": 141,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 128,
        "is_required": 0,
        "name": "资料名称",
        "no": 4,
        "parent_no": 141,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 129,
        "is_required": 0,
        "name": "手册编制的参考标准",
        "no": 5,
        "parent_no": 141,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 130,
        "is_required": 0,
        "name": "主要用途/使用时机/使用地点",
        "no": 6,
        "parent_no": 141,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 131,
        "is_required": 0,
        "name": "主要使用对象",
        "no": 7,
        "parent_no": 141,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 132,
        "is_required": 0,
        "name": "扩展",
        "no": 8,
        "parent_no": 141,
        "restriction": null,
        "template_id": null,
        "type_no": 275
    },
    {
        "description": null,
        "id": 133,
        "is_required": 0,
        "name": "备注",
        "no": 9,
        "parent_no": 141,
        "restriction": null,
        "template_id": null,
        "type_no": 2
    },
    {
        "description": "主键",
        "id": 134,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 142,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 135,
        "is_required": 0,
        "name": "基线id",
        "no": 1,
        "parent_no": 142,
        "restriction": null,
        "template_id": null,
        "type_no": 70
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 136,
        "is_required": 0,
        "name": "previous_version_id",
        "no": 2,
        "parent_no": 142,
        "restriction": null,
        "template_id": null,
        "type_no": 78
    },
    {
        "description": "技术状态文件的类型",
        "id": 137,
        "is_required": 0,
        "name": "类型",
        "no": 3,
        "parent_no": 142,
        "restriction": null,
        "template_id": null,
        "type_no": 276
    },
    {
        "description": "文件存储路径",
        "id": 138,
        "is_required": 0,
        "name": "路径",
        "no": 4,
        "parent_no": 142,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "技术状态文件的编号",
        "id": 139,
        "is_required": 0,
        "name": "编号",
        "no": 5,
        "parent_no": 142,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 140,
        "is_required": 0,
        "name": "名称",
        "no": 6,
        "parent_no": 142,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 141,
        "is_required": 0,
        "name": "版本",
        "no": 7,
        "parent_no": 142,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "2006-01-02 15:04:05",
        "id": 142,
        "is_required": 0,
        "name": "修订时间",
        "no": 8,
        "parent_no": 142,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 143,
        "is_required": 0,
        "name": "构型节点id",
        "no": 9,
        "parent_no": 142,
        "restriction": null,
        "template_id": null,
        "type_no": 72
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"type\"}",
        "id": 144,
        "is_required": 0,
        "name": "附件",
        "no": 10,
        "parent_no": 142,
        "restriction": null,
        "template_id": null,
        "type_no": 8
    },
    {
        "description": null,
        "id": 145,
        "is_required": 0,
        "name": "state",
        "no": 11,
        "parent_no": 142,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "技术状态文件的扩展数据",
        "id": 146,
        "is_required": 0,
        "name": "扩展",
        "no": 12,
        "parent_no": 142,
        "restriction": null,
        "template_id": null,
        "type_no": 277
    },
    {
        "description": null,
        "id": 147,
        "is_required": 0,
        "name": "备注",
        "no": 13,
        "parent_no": 142,
        "restriction": null,
        "template_id": null,
        "type_no": 2
    },
    {
        "description": "主键",
        "id": 148,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 143,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 149,
        "is_required": 0,
        "name": "基线id",
        "no": 1,
        "parent_no": 143,
        "restriction": null,
        "template_id": null,
        "type_no": 70
    },
    {
        "description": null,
        "id": 150,
        "is_required": 0,
        "name": "扩展",
        "no": 2,
        "parent_no": 143,
        "restriction": null,
        "template_id": null,
        "type_no": 278
    },
    {
        "description": "主键",
        "id": 151,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 144,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 152,
        "is_required": 0,
        "name": "基线id",
        "no": 1,
        "parent_no": 144,
        "restriction": null,
        "template_id": null,
        "type_no": 70
    },
    {
        "description": null,
        "id": 153,
        "is_required": 0,
        "name": "扩展",
        "no": 2,
        "parent_no": 144,
        "restriction": null,
        "template_id": null,
        "type_no": 279
    },
    {
        "description": "主键",
        "id": 154,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 145,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 155,
        "is_required": 0,
        "name": "基线id",
        "no": 1,
        "parent_no": 145,
        "restriction": null,
        "template_id": null,
        "type_no": 70
    },
    {
        "description": null,
        "id": 156,
        "is_required": 0,
        "name": "扩展",
        "no": 2,
        "parent_no": 145,
        "restriction": null,
        "template_id": null,
        "type_no": 280
    },
    {
        "description": "主键",
        "id": 157,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 146,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 158,
        "is_required": 0,
        "name": "基线id",
        "no": 1,
        "parent_no": 146,
        "restriction": null,
        "template_id": null,
        "type_no": 70
    },
    {
        "description": null,
        "id": 159,
        "is_required": 0,
        "name": "time",
        "no": 2,
        "parent_no": 146,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 160,
        "is_required": 0,
        "name": "no",
        "no": 3,
        "parent_no": 146,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 161,
        "is_required": 0,
        "name": "control_file_id",
        "no": 4,
        "parent_no": 146,
        "restriction": null,
        "template_id": null,
        "type_no": 78
    },
    {
        "description": null,
        "id": 162,
        "is_required": 0,
        "name": "description",
        "no": 5,
        "parent_no": 146,
        "restriction": null,
        "template_id": null,
        "type_no": 2
    },
    {
        "description": "主键",
        "id": 163,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 147,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 164,
        "is_required": 0,
        "name": "change_history_id",
        "no": 1,
        "parent_no": 147,
        "restriction": null,
        "template_id": null,
        "type_no": 99
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 165,
        "is_required": 0,
        "name": "from_file_id",
        "no": 2,
        "parent_no": 147,
        "restriction": null,
        "template_id": null,
        "type_no": 78
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 166,
        "is_required": 0,
        "name": "to_file_id",
        "no": 3,
        "parent_no": 147,
        "restriction": null,
        "template_id": null,
        "type_no": 78
    },
    {
        "description": null,
        "id": 167,
        "is_required": 0,
        "name": "description",
        "no": 4,
        "parent_no": 147,
        "restriction": null,
        "template_id": null,
        "type_no": 2
    },
    {
        "description": "主键",
        "id": 168,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 148,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": null,
        "id": 169,
        "is_required": 0,
        "name": "type",
        "no": 1,
        "parent_no": 148,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 170,
        "is_required": 0,
        "name": "ext",
        "no": 2,
        "parent_no": 148,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 171,
        "is_required": 0,
        "name": "content",
        "no": 3,
        "parent_no": 148,
        "restriction": null,
        "template_id": null,
        "type_no": 8
    },
    {
        "description": null,
        "id": 172,
        "is_required": 0,
        "name": "size",
        "no": 4,
        "parent_no": 148,
        "restriction": null,
        "template_id": null,
        "type_no": 4
    },
    {
        "description": null,
        "id": 173,
        "is_required": 0,
        "name": "digest",
        "no": 5,
        "parent_no": 148,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "主键",
        "id": 174,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 149,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": null,
        "id": 175,
        "is_required": 0,
        "name": "template_id",
        "no": 1,
        "parent_no": 149,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": null,
        "id": 176,
        "is_required": 0,
        "name": "parent_id",
        "no": 2,
        "parent_no": 149,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": null,
        "id": 177,
        "is_required": 0,
        "name": "name",
        "no": 3,
        "parent_no": 149,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 178,
        "is_required": 0,
        "name": "table_no",
        "no": 4,
        "parent_no": 149,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": null,
        "id": 179,
        "is_required": 0,
        "name": "table_field_no",
        "no": 5,
        "parent_no": 149,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "主键",
        "id": 180,
        "is_required": 1,
        "name": "id",
        "no": 0,
        "parent_no": 150,
        "restriction": null,
        "template_id": null,
        "type_no": 3
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 181,
        "is_required": 0,
        "name": "基线id",
        "no": 1,
        "parent_no": 150,
        "restriction": null,
        "template_id": null,
        "type_no": 70
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 182,
        "is_required": 0,
        "name": "parent_id",
        "no": 2,
        "parent_no": 150,
        "restriction": null,
        "template_id": null,
        "type_no": 72
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\"}",
        "id": 183,
        "is_required": 0,
        "name": "构型节点id",
        "no": 3,
        "parent_no": 150,
        "restriction": null,
        "template_id": null,
        "type_no": 72
    },
    {
        "description": null,
        "id": 184,
        "is_required": 0,
        "name": "名称",
        "no": 5,
        "parent_no": 150,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 185,
        "is_required": 0,
        "name": "版本",
        "no": 6,
        "parent_no": 150,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": "{\"value\": \"id\", \"show\": \"名称\", \"filter\": \"\\\"类型\\\" = 1\"}",
        "id": 186,
        "is_required": 0,
        "name": "配套厂id",
        "no": 9,
        "parent_no": 150,
        "restriction": null,
        "template_id": null,
        "type_no": 65
    },
    {
        "description": null,
        "id": 187,
        "is_required": 0,
        "name": "重要度",
        "no": 10,
        "parent_no": 150,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 188,
        "is_required": 0,
        "name": "状态",
        "no": 11,
        "parent_no": 150,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 189,
        "is_required": 0,
        "name": "主管",
        "no": 12,
        "parent_no": 150,
        "restriction": null,
        "template_id": null,
        "type_no": 1
    },
    {
        "description": null,
        "id": 190,
        "is_required": 0,
        "name": "extended",
        "no": 13,
        "parent_no": 150,
        "restriction": null,
        "template_id": null,
        "type_no": 281
    },
    {
        "description": null,
        "id": 191,
        "is_required": 0,
        "name": "备注",
        "no": 14,
        "parent_no": 150,
        "restriction": null,
        "template_id": null,
        "type_no": 2
    },
    {
        "description": null,
        "id": 192,
        "is_required": 0,
        "name": "启动布局",
        "no": 0,
        "parent_no": 262,
        "restriction": null,
        "template_id": 1,
        "type_no": 2
    },
    {
        "description": null,
        "id": 193,
        "is_required": 0,
        "name": "总体配置",
        "no": 1,
        "parent_no": 262,
        "restriction": null,
        "template_id": 1,
        "type_no": 2
    },
    {
        "description": null,
        "id": 194,
        "is_required": 0,
        "name": "图片",
        "no": 2,
        "parent_no": 262,
        "restriction": null,
        "template_id": 1,
        "type_no": 1025
    },
    {
        "description": null,
        "id": 195,
        "is_required": 0,
        "name": "外型参数",
        "no": 0,
        "parent_no": 263,
        "restriction": null,
        "template_id": 1,
        "type_no": 1026
    },
    {
        "description": null,
        "id": 196,
        "is_required": 0,
        "name": "重量重心",
        "no": 1,
        "parent_no": 263,
        "restriction": null,
        "template_id": 1,
        "type_no": 2
    },
    {
        "description": null,
        "id": 197,
        "is_required": 0,
        "name": "飞行性能",
        "no": 2,
        "parent_no": 263,
        "restriction": null,
        "template_id": 1,
        "type_no": 1027
    },
    {
        "description": null,
        "id": 198,
        "is_required": 0,
        "name": "隐身措施",
        "no": 3,
        "parent_no": 263,
        "restriction": null,
        "template_id": 1,
        "type_no": 2
    },
    {
        "description": null,
        "id": 199,
        "is_required": 0,
        "name": "高原措施",
        "no": 4,
        "parent_no": 263,
        "restriction": null,
        "template_id": 1,
        "type_no": 2
    },
    {
        "description": null,
        "id": 200,
        "is_required": 0,
        "name": "寿命",
        "no": 5,
        "parent_no": 263,
        "restriction": null,
        "template_id": 1,
        "type_no": 1028
    },
    {
        "description": null,
        "id": 201,
        "is_required": 0,
        "name": "武器",
        "no": 0,
        "parent_no": 264,
        "restriction": null,
        "template_id": 1,
        "type_no": 1029
    },
    {
        "description": null,
        "id": 202,
        "is_required": 0,
        "name": "动力装置",
        "no": 1,
        "parent_no": 264,
        "restriction": null,
        "template_id": 1,
        "type_no": 1030
    },
    {
        "description": null,
        "id": 203,
        "is_required": 0,
        "name": "雷达配置",
        "no": 2,
        "parent_no": 264,
        "restriction": null,
        "template_id": 1,
        "type_no": 1031
    },
    {
        "description": null,
        "id": 204,
        "is_required": 0,
        "name": "电子对抗",
        "no": 3,
        "parent_no": 264,
        "restriction": null,
        "template_id": 1,
        "type_no": 1032
    },
    {
        "description": null,
        "id": 205,
        "is_required": 0,
        "name": "外挂吊仓",
        "no": 4,
        "parent_no": 264,
        "restriction": null,
        "template_id": 1,
        "type_no": 1033
    },
    {
        "description": null,
        "id": 206,
        "is_required": 0,
        "name": "数据链",
        "no": 5,
        "parent_no": 264,
        "restriction": null,
        "template_id": 1,
        "type_no": 1034
    },
    {
        "description": null,
        "id": 207,
        "is_required": 0,
        "name": "座椅",
        "no": 6,
        "parent_no": 264,
        "restriction": null,
        "template_id": 1,
        "type_no": 1035
    },
    {
        "description": null,
        "id": 208,
        "is_required": 0,
        "name": "特殊使用限制",
        "no": 7,
        "parent_no": 264,
        "restriction": null,
        "template_id": 1,
        "type_no": 2
    },
    {
        "description": null,
        "id": 209,
        "is_required": 0,
        "name": "1:1",
        "no": 0,
        "parent_no": 270,
        "restriction": "{\"max\": 8}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 210,
        "is_required": 0,
        "name": "1:4",
        "no": 1,
        "parent_no": 270,
        "restriction": "{\"max\": 8}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 211,
        "is_required": 0,
        "name": "1:12",
        "no": 2,
        "parent_no": 270,
        "restriction": "{\"max\": 8}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 212,
        "is_required": 0,
        "name": "1:1",
        "no": 0,
        "parent_no": 272,
        "restriction": "{\"max\": 8}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 213,
        "is_required": 0,
        "name": "1:8",
        "no": 1,
        "parent_no": 272,
        "restriction": "{\"max\": 8}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 214,
        "is_required": 0,
        "name": "1:24",
        "no": 2,
        "parent_no": 272,
        "restriction": "{\"max\": 8}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 215,
        "is_required": 0,
        "name": "型号",
        "no": 0,
        "parent_no": 278,
        "restriction": "{\"max\": 16}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 216,
        "is_required": 0,
        "name": "名称",
        "no": 1,
        "parent_no": 278,
        "restriction": "{\"max\": 16}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 217,
        "is_required": 0,
        "name": "配套厂",
        "no": 2,
        "parent_no": 278,
        "restriction": "{\"max\": 16}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 218,
        "is_required": 0,
        "name": "数量",
        "no": 3,
        "parent_no": 278,
        "restriction": null,
        "template_id": 1,
        "type_no": 4
    },
    {
        "description": null,
        "id": 219,
        "is_required": 0,
        "name": "寿命",
        "no": 4,
        "parent_no": 278,
        "restriction": null,
        "template_id": 1,
        "type_no": 3
    },
    {
        "description": null,
        "id": 220,
        "is_required": 0,
        "name": "双",
        "no": 0,
        "parent_no": 279,
        "restriction": "{\"max\": 8}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 221,
        "is_required": 0,
        "name": "流",
        "no": 1,
        "parent_no": 279,
        "restriction": "{\"max\": 8}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 222,
        "is_required": 0,
        "name": "水",
        "no": 2,
        "parent_no": 279,
        "restriction": "{\"max\": 8}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 223,
        "is_required": 0,
        "name": "编号",
        "no": 0,
        "parent_no": 280,
        "restriction": "{\"max\": 8}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 224,
        "is_required": 0,
        "name": "描述",
        "no": 1,
        "parent_no": 280,
        "restriction": null,
        "template_id": 1,
        "type_no": 2
    },
    {
        "description": null,
        "id": 225,
        "is_required": 0,
        "name": "相关产品",
        "no": 2,
        "parent_no": 280,
        "restriction": null,
        "template_id": 1,
        "type_no": 2
    },
    {
        "description": null,
        "id": 226,
        "is_required": 0,
        "name": "严重程度",
        "no": 3,
        "parent_no": 280,
        "restriction": null,
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 227,
        "is_required": 0,
        "name": "处置情况",
        "no": 4,
        "parent_no": 280,
        "restriction": null,
        "template_id": 1,
        "type_no": 2
    },
    {
        "description": null,
        "id": 228,
        "is_required": 0,
        "name": "机长",
        "no": 0,
        "parent_no": 1026,
        "restriction": "{\"unitId\": 28}",
        "template_id": 1,
        "type_no": 5
    },
    {
        "description": null,
        "id": 229,
        "is_required": 0,
        "name": "翼展",
        "no": 1,
        "parent_no": 1026,
        "restriction": "{\"unitId\": 28}",
        "template_id": 1,
        "type_no": 5
    },
    {
        "description": null,
        "id": 230,
        "is_required": 0,
        "name": "机高",
        "no": 2,
        "parent_no": 1026,
        "restriction": "{\"unitId\": 28}",
        "template_id": 1,
        "type_no": 5
    },
    {
        "description": null,
        "id": 231,
        "is_required": 0,
        "name": "机翼面积",
        "no": 3,
        "parent_no": 1026,
        "restriction": "{\"unitId\": 35}",
        "template_id": 1,
        "type_no": 5
    },
    {
        "description": null,
        "id": 232,
        "is_required": 0,
        "name": "主轮距",
        "no": 4,
        "parent_no": 1026,
        "restriction": "{\"unitId\": 28}",
        "template_id": 1,
        "type_no": 5
    },
    {
        "description": null,
        "id": 233,
        "is_required": 0,
        "name": "前主轮距",
        "no": 5,
        "parent_no": 1026,
        "restriction": "{\"unitId\": 28}",
        "template_id": 1,
        "type_no": 5
    },
    {
        "description": null,
        "id": 234,
        "is_required": 0,
        "name": "实用升限",
        "no": 0,
        "parent_no": 1027,
        "restriction": "{\"max\": 8, \"unitId\": 27}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 235,
        "is_required": 0,
        "name": "最大飞行M数",
        "no": 1,
        "parent_no": 1027,
        "restriction": "{\"max\": 8}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 236,
        "is_required": 0,
        "name": "最大飞行表数",
        "no": 2,
        "parent_no": 1027,
        "restriction": "{\"max\": 8, \"unitId\": 76}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 237,
        "is_required": 0,
        "name": "重量",
        "no": 3,
        "parent_no": 1027,
        "restriction": "{\"max\": 8, \"unitId\": 50}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 238,
        "is_required": 0,
        "name": "起飞滑跑距离",
        "no": 4,
        "parent_no": 1027,
        "restriction": "{\"max\": 8, \"unitId\": 28}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 239,
        "is_required": 0,
        "name": "着陆滑跑距离",
        "no": 5,
        "parent_no": 1027,
        "restriction": "{\"max\": 8, \"unitId\": 28}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 240,
        "is_required": 0,
        "name": "机动性能",
        "no": 6,
        "parent_no": 1027,
        "restriction": "{\"max\": 8}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 241,
        "is_required": 0,
        "name": "航程航时",
        "no": 7,
        "parent_no": 1027,
        "restriction": "{\"max\": 8}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 242,
        "is_required": 0,
        "name": "机体使用寿命",
        "no": 0,
        "parent_no": 1028,
        "restriction": "{\"max\": 8, \"unitId\": 54}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 243,
        "is_required": 0,
        "name": "机体日历寿命",
        "no": 1,
        "parent_no": 1028,
        "restriction": "{\"max\": 8, \"unitId\": 54}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 244,
        "is_required": 0,
        "name": "大修时间间隔",
        "no": 2,
        "parent_no": 1028,
        "restriction": "{\"max\": 8}",
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 245,
        "is_required": 0,
        "name": "武器外挂",
        "no": 0,
        "parent_no": 1029,
        "restriction": null,
        "template_id": 1,
        "type_no": 1036
    },
    {
        "description": null,
        "id": 246,
        "is_required": 0,
        "name": "挂梁挂柱",
        "no": 1,
        "parent_no": 1029,
        "restriction": null,
        "template_id": 1,
        "type_no": 1037
    },
    {
        "description": null,
        "id": 247,
        "is_required": 0,
        "name": "挂点+外挂物种类",
        "no": 1,
        "parent_no": 1029,
        "restriction": null,
        "template_id": 1,
        "type_no": 2
    },
    {
        "description": null,
        "id": 248,
        "is_required": 0,
        "name": "型号",
        "no": 0,
        "parent_no": 1030,
        "restriction": null,
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 249,
        "is_required": 0,
        "name": "数量",
        "no": 1,
        "parent_no": 1030,
        "restriction": null,
        "template_id": 1,
        "type_no": 4
    },
    {
        "description": null,
        "id": 250,
        "is_required": 0,
        "name": "总寿命",
        "no": 2,
        "parent_no": 1030,
        "restriction": null,
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 251,
        "is_required": 0,
        "name": "翻修周期",
        "no": 3,
        "parent_no": 1030,
        "restriction": null,
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 252,
        "is_required": 0,
        "name": "翻修单位",
        "no": 4,
        "parent_no": 1030,
        "restriction": null,
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 253,
        "is_required": 0,
        "name": "型号",
        "no": 0,
        "parent_no": 1031,
        "restriction": null,
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 254,
        "is_required": 0,
        "name": "主要性能",
        "no": 1,
        "parent_no": 1031,
        "restriction": null,
        "template_id": 1,
        "type_no": 2
    },
    {
        "description": null,
        "id": 255,
        "is_required": 0,
        "name": "型号发展",
        "no": 2,
        "parent_no": 1031,
        "restriction": null,
        "template_id": 1,
        "type_no": 2
    },
    {
        "description": null,
        "id": 256,
        "is_required": 0,
        "name": "型号",
        "no": 0,
        "parent_no": 1032,
        "restriction": null,
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 257,
        "is_required": 0,
        "name": "主要性能",
        "no": 1,
        "parent_no": 1032,
        "restriction": null,
        "template_id": 1,
        "type_no": 2
    },
    {
        "description": null,
        "id": 258,
        "is_required": 0,
        "name": "型号发展",
        "no": 2,
        "parent_no": 1032,
        "restriction": null,
        "template_id": 1,
        "type_no": 2
    },
    {
        "description": null,
        "id": 259,
        "is_required": 0,
        "name": "型号",
        "no": 0,
        "parent_no": 1033,
        "restriction": null,
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 260,
        "is_required": 0,
        "name": "数量",
        "no": 1,
        "parent_no": 1033,
        "restriction": null,
        "template_id": 1,
        "type_no": 3
    },
    {
        "description": null,
        "id": 261,
        "is_required": 0,
        "name": "型号发展",
        "no": 2,
        "parent_no": 1033,
        "restriction": null,
        "template_id": 1,
        "type_no": 2
    },
    {
        "description": null,
        "id": 262,
        "is_required": 0,
        "name": "型号",
        "no": 0,
        "parent_no": 1035,
        "restriction": null,
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 263,
        "is_required": 0,
        "name": "航炮",
        "no": 0,
        "parent_no": 1038,
        "restriction": null,
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 264,
        "is_required": 0,
        "name": "制导弹药",
        "no": 1,
        "parent_no": 1038,
        "restriction": null,
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 265,
        "is_required": 0,
        "name": "常规弹药",
        "no": 2,
        "parent_no": 1038,
        "restriction": null,
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 266,
        "is_required": 0,
        "name": "配套基数",
        "no": 3,
        "parent_no": 1038,
        "restriction": null,
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 267,
        "is_required": 0,
        "name": "挂载方案",
        "no": 4,
        "parent_no": 1038,
        "restriction": null,
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 268,
        "is_required": 0,
        "name": "名称",
        "no": 0,
        "parent_no": 1039,
        "restriction": null,
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 269,
        "is_required": 0,
        "name": "型号",
        "no": 1,
        "parent_no": 1039,
        "restriction": null,
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 270,
        "is_required": 0,
        "name": "数量",
        "no": 2,
        "parent_no": 1039,
        "restriction": null,
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 271,
        "is_required": 0,
        "name": "配套弹药",
        "no": 3,
        "parent_no": 1039,
        "restriction": null,
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 272,
        "is_required": 0,
        "name": "名称",
        "no": 0,
        "parent_no": 1040,
        "restriction": null,
        "template_id": 1,
        "type_no": 1
    },
    {
        "description": null,
        "id": 273,
        "is_required": 0,
        "name": "图片",
        "no": 1,
        "parent_no": 1040,
        "restriction": null,
        "template_id": 1,
        "type_no": 9
    }
  ];

  const lists = [
    {
        "description": null,
        "element_type_no": 129,
        "group_by": "类型",
        "id": 1,
        "is_hidden": 1,
        "max": -1,
        "min": 0,
        "parent_no": 65,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 130,
        "group_by": null,
        "id": 2,
        "is_hidden": 1,
        "max": -1,
        "min": 0,
        "parent_no": 66,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 131,
        "group_by": null,
        "id": 3,
        "is_hidden": 0,
        "max": -1,
        "min": 0,
        "parent_no": 67,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 132,
        "group_by": null,
        "id": 4,
        "is_hidden": 0,
        "max": -1,
        "min": 0,
        "parent_no": 68,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 133,
        "group_by": null,
        "id": 5,
        "is_hidden": 0,
        "max": -1,
        "min": 0,
        "parent_no": 69,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 134,
        "group_by": null,
        "id": 6,
        "is_hidden": 1,
        "max": -1,
        "min": 0,
        "parent_no": 70,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 135,
        "group_by": "tag",
        "id": 7,
        "is_hidden": 0,
        "max": -1,
        "min": 0,
        "parent_no": 71,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 136,
        "group_by": null,
        "id": 8,
        "is_hidden": 0,
        "max": -1,
        "min": 0,
        "parent_no": 72,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 137,
        "group_by": null,
        "id": 9,
        "is_hidden": 1,
        "max": -1,
        "min": 0,
        "parent_no": 73,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 138,
        "group_by": "专业",
        "id": 10,
        "is_hidden": 0,
        "max": -1,
        "min": 0,
        "parent_no": 74,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 139,
        "group_by": null,
        "id": 11,
        "is_hidden": 0,
        "max": -1,
        "min": 0,
        "parent_no": 75,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 140,
        "group_by": null,
        "id": 12,
        "is_hidden": 0,
        "max": -1,
        "min": 0,
        "parent_no": 76,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 141,
        "group_by": null,
        "id": 13,
        "is_hidden": 0,
        "max": -1,
        "min": 0,
        "parent_no": 77,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 142,
        "group_by": "类型",
        "id": 14,
        "is_hidden": 0,
        "max": -1,
        "min": 0,
        "parent_no": 78,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 143,
        "group_by": null,
        "id": 15,
        "is_hidden": 0,
        "max": -1,
        "min": 0,
        "parent_no": 79,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 144,
        "group_by": null,
        "id": 16,
        "is_hidden": 0,
        "max": -1,
        "min": 0,
        "parent_no": 80,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 145,
        "group_by": null,
        "id": 17,
        "is_hidden": 0,
        "max": -1,
        "min": 0,
        "parent_no": 81,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 146,
        "group_by": null,
        "id": 18,
        "is_hidden": 1,
        "max": -1,
        "min": 0,
        "parent_no": 82,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 147,
        "group_by": null,
        "id": 19,
        "is_hidden": 1,
        "max": -1,
        "min": 0,
        "parent_no": 83,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 148,
        "group_by": null,
        "id": 20,
        "is_hidden": 1,
        "max": -1,
        "min": 0,
        "parent_no": 84,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 149,
        "group_by": null,
        "id": 21,
        "is_hidden": 1,
        "max": -1,
        "min": 0,
        "parent_no": 85,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 150,
        "group_by": null,
        "id": 22,
        "is_hidden": 1,
        "max": -1,
        "min": 0,
        "parent_no": 86,
        "template_id": null
    },
    {
        "description": null,
        "element_type_no": 1040,
        "group_by": null,
        "id": 23,
        "is_hidden": 0,
        "max": -1,
        "min": 0,
        "parent_no": 1025,
        "template_id": 1
    },
    {
        "description": null,
        "element_type_no": 1038,
        "group_by": null,
        "id": 24,
        "is_hidden": 0,
        "max": -1,
        "min": 0,
        "parent_no": 1036,
        "template_id": 1
    },
    {
        "description": null,
        "element_type_no": 1039,
        "group_by": null,
        "id": 25,
        "is_hidden": 0,
        "max": -1,
        "min": 0,
        "parent_no": 1037,
        "template_id": 1
    }
  ];

  const unitTypes = [
    {
      "id": 1,
      "name": "量词",
      "description": null
    },
    {
      "id": 2,
      "name": "长度",
      "description": null
    },
    {
      "id": 3,
      "name": "面积",
      "description": null
    },
    {
      "id": 4,
      "name": "体积",
      "description": null
    },
    {
      "id": 5,
      "name": "质量",
      "description": null
    },
    {
      "id": 6,
      "name": "时间",
      "description": null
    },
    {
      "id": 7,
      "name": "平面角",
      "description": null
    },
    {
      "id": 8,
      "name": "密度",
      "description": null
    },
    {
      "id": 9,
      "name": "压力",
      "description": null
    },
    {
      "id": 10,
      "name": "力",
      "description": null
    },
    {
      "id": 11,
      "name": "温度",
      "description": null
    },
    {
      "id": 12,
      "name": "传导系数",
      "description": null
    },
    {
      "id": 13,
      "name": "比热容",
      "description": null
    },
    {
      "id": 14,
      "name": "能量、功、热",
      "description": null
    },
    {
      "id": 15,
      "name": "功率、辐射能量",
      "description": null
    },
    {
      "id": 16,
      "name": "速度",
      "description": null
    },
    {
      "id": 17,
      "name": "电容",
      "description": null
    },
    {
      "id": 18,
      "name": "电阻",
      "description": null
    },
    {
      "id": 19,
      "name": "流量",
      "description": null
    },
    {
      "id": 20,
      "name": "频率",
      "description": null
    },
    {
      "id": 21,
      "name": "电荷量",
      "description": null
    },
    {
      "id": 22,
      "name": "电势、电压、电动势",
      "description": null
    },
    {
      "id": 23,
      "name": "电导",
      "description": null
    },
    {
      "id": 24,
      "name": "磁通量",
      "description": null
    },
    {
      "id": 25,
      "name": "磁通量密度",
      "description": null
    },
    {
      "id": 26,
      "name": "电感",
      "description": null
    },
    {
      "id": 27,
      "name": "光通量",
      "description": null
    },
    {
      "id": 28,
      "name": "光照度",
      "description": null
    },
    {
      "id": 29,
      "name": "放射性活度",
      "description": null
    },
    {
      "id": 30,
      "name": "吸收当量",
      "description": null
    },
    {
      "id": 31,
      "name": "剂量当量",
      "description": null
    },
    {
      "id": 32,
      "name": "旋转速度",
      "description": null
    },
    {
      "id": 33,
      "name": "能",
      "description": null
    },
    {
      "id": 34,
      "name": "级差",
      "description": null
    },
    {
      "id": 35,
      "name": "线密度",
      "description": null
    }
  ];

  const units = [
    {
      "id": 1,
      "type_id": 1,
      "name": "个",
      "symbol": "个",
      "description": null
    },
    {
      "id": 2,
      "type_id": 1,
      "name": "件",
      "symbol": "件",
      "description": null
    },
    {
      "id": 3,
      "type_id": 1,
      "name": "辆",
      "symbol": "辆",
      "description": null
    },
    {
      "id": 4,
      "type_id": 1,
      "name": "架",
      "symbol": "架",
      "description": null
    },
    {
      "id": 5,
      "type_id": 1,
      "name": "台",
      "symbol": "台",
      "description": null
    },
    {
      "id": 6,
      "type_id": 1,
      "name": "片",
      "symbol": "片",
      "description": null
    },
    {
      "id": 7,
      "type_id": 1,
      "name": "颗",
      "symbol": "颗",
      "description": null
    },
    {
      "id": 8,
      "type_id": 1,
      "name": "粒",
      "symbol": "粒",
      "description": null
    },
    {
      "id": 9,
      "type_id": 1,
      "name": "门",
      "symbol": "门",
      "description": null
    },
    {
      "id": 10,
      "type_id": 1,
      "name": "只",
      "symbol": "只",
      "description": null
    },
    {
      "id": 11,
      "type_id": 1,
      "name": "双",
      "symbol": "双",
      "description": null
    },
    {
      "id": 12,
      "type_id": 1,
      "name": "棵",
      "symbol": "棵",
      "description": null
    },
    {
      "id": 13,
      "type_id": 1,
      "name": "张",
      "symbol": "张",
      "description": null
    },
    {
      "id": 14,
      "type_id": 1,
      "name": "串",
      "symbol": "串",
      "description": null
    },
    {
      "id": 15,
      "type_id": 1,
      "name": "把",
      "symbol": "把",
      "description": null
    },
    {
      "id": 16,
      "type_id": 1,
      "name": "群",
      "symbol": "群",
      "description": null
    },
    {
      "id": 17,
      "type_id": 1,
      "name": "本",
      "symbol": "本",
      "description": null
    },
    {
      "id": 18,
      "type_id": 1,
      "name": "匹",
      "symbol": "匹",
      "description": null
    },
    {
      "id": 19,
      "type_id": 1,
      "name": "条",
      "symbol": "条",
      "description": null
    },
    {
      "id": 20,
      "type_id": 1,
      "name": "部",
      "symbol": "部",
      "description": null
    },
    {
      "id": 21,
      "type_id": 1,
      "name": "艘",
      "symbol": "艘",
      "description": null
    },
    {
      "id": 22,
      "type_id": 1,
      "name": "间",
      "symbol": "间",
      "description": null
    },
    {
      "id": 23,
      "type_id": 1,
      "name": "束",
      "symbol": "束",
      "description": null
    },
    {
      "id": 24,
      "type_id": 1,
      "name": "幅",
      "symbol": "幅",
      "description": null
    },
    {
      "id": 25,
      "type_id": 1,
      "name": "种",
      "symbol": "种",
      "description": null
    },
    {
      "id": 26,
      "type_id": 1,
      "name": "杯",
      "symbol": "杯",
      "description": null
    },
    {
      "id": 27,
      "type_id": 2,
      "name": "千米",
      "symbol": "Km",
      "description": null
    },
    {
      "id": 28,
      "type_id": 2,
      "name": "米",
      "symbol": "m",
      "description": null
    },
    {
      "id": 29,
      "type_id": 2,
      "name": "分米",
      "symbol": "dm",
      "description": null
    },
    {
      "id": 30,
      "type_id": 2,
      "name": "厘米",
      "symbol": "cm",
      "description": null
    },
    {
      "id": 31,
      "type_id": 2,
      "name": "毫米",
      "symbol": "mm",
      "description": null
    },
    {
      "id": 32,
      "type_id": 2,
      "name": "微米",
      "symbol": "um",
      "description": null
    },
    {
      "id": 33,
      "type_id": 2,
      "name": "海里",
      "symbol": "mile",
      "description": null
    },
    {
      "id": 34,
      "type_id": 3,
      "name": "平方千米",
      "symbol": "Km2",
      "description": null
    },
    {
      "id": 35,
      "type_id": 3,
      "name": "平方米",
      "symbol": "m2",
      "description": null
    },
    {
      "id": 36,
      "type_id": 3,
      "name": "平方分米",
      "symbol": "dm2",
      "description": null
    },
    {
      "id": 37,
      "type_id": 3,
      "name": "平方厘米",
      "symbol": "cm2",
      "description": null
    },
    {
      "id": 38,
      "type_id": 3,
      "name": "平方毫米",
      "symbol": "mm2",
      "description": null
    },
    {
      "id": 39,
      "type_id": 3,
      "name": "平方微米",
      "symbol": "um2",
      "description": null
    },
    {
      "id": 40,
      "type_id": 3,
      "name": "公顷",
      "symbol": "ha",
      "description": null
    },
    {
      "id": 41,
      "type_id": 4,
      "name": "立方千米",
      "symbol": "Km3",
      "description": null
    },
    {
      "id": 42,
      "type_id": 4,
      "name": "立方米",
      "symbol": "m3",
      "description": null
    },
    {
      "id": 43,
      "type_id": 4,
      "name": "立方分米",
      "symbol": "dm3",
      "description": null
    },
    {
      "id": 44,
      "type_id": 4,
      "name": "立方厘米",
      "symbol": "cm3",
      "description": null
    },
    {
      "id": 45,
      "type_id": 4,
      "name": "立方毫米",
      "symbol": "mm3",
      "description": null
    },
    {
      "id": 46,
      "type_id": 4,
      "name": "立方微米",
      "symbol": "um3",
      "description": null
    },
    {
      "id": 47,
      "type_id": 4,
      "name": "升",
      "symbol": "L",
      "description": null
    },
    {
      "id": 48,
      "type_id": 4,
      "name": "毫升",
      "symbol": "mL",
      "description": null
    },
    {
      "id": 49,
      "type_id": 5,
      "name": "吨",
      "symbol": "t",
      "description": null
    },
    {
      "id": 50,
      "type_id": 5,
      "name": "千克",
      "symbol": "Kg",
      "description": null
    },
    {
      "id": 51,
      "type_id": 5,
      "name": "克",
      "symbol": "g",
      "description": null
    },
    {
      "id": 52,
      "type_id": 6,
      "name": "秒",
      "symbol": "s",
      "description": null
    },
    {
      "id": 53,
      "type_id": 6,
      "name": "分",
      "symbol": "min",
      "description": null
    },
    {
      "id": 54,
      "type_id": 6,
      "name": "小时",
      "symbol": "h",
      "description": null
    },
    {
      "id": 55,
      "type_id": 7,
      "name": "秒",
      "symbol": "“",
      "description": null
    },
    {
      "id": 56,
      "type_id": 7,
      "name": "分",
      "symbol": "‘",
      "description": null
    },
    {
      "id": 57,
      "type_id": 7,
      "name": "度",
      "symbol": "˚",
      "description": null
    },
    {
      "id": 58,
      "type_id": 8,
      "name": "千克/立方米",
      "symbol": "Kg/m3",
      "description": null
    },
    {
      "id": 59,
      "type_id": 8,
      "name": "克/立方厘米",
      "symbol": "g/cm3",
      "description": null
    },
    {
      "id": 60,
      "type_id": 9,
      "name": "兆帕",
      "symbol": "Mpa",
      "description": null
    },
    {
      "id": 61,
      "type_id": 9,
      "name": "帕",
      "symbol": "Pa",
      "description": null
    },
    {
      "id": 62,
      "type_id": 9,
      "name": "巴",
      "symbol": "b",
      "description": null
    },
    {
      "id": 63,
      "type_id": 9,
      "name": "大气压",
      "symbol": "at m",
      "description": null
    },
    {
      "id": 64,
      "type_id": 10,
      "name": "力",
      "symbol": "N",
      "description": null
    },
    {
      "id": 65,
      "type_id": 11,
      "name": "开尔文度",
      "symbol": "K",
      "description": null
    },
    {
      "id": 66,
      "type_id": 11,
      "name": "摄氏度",
      "symbol": "℃",
      "description": null
    },
    {
      "id": 67,
      "type_id": 11,
      "name": "华氏度",
      "symbol": "℉",
      "description": null
    },
    {
      "id": 68,
      "type_id": 12,
      "name": "千卡",
      "symbol": "kcal/m2.h.℃",
      "description": null
    },
    {
      "id": 69,
      "type_id": 13,
      "name": "千卡/千克.℃",
      "symbol": "kcal/kg.℃",
      "description": null
    },
    {
      "id": 70,
      "type_id": 14,
      "name": "焦耳",
      "symbol": "J",
      "description": null
    },
    {
      "id": 71,
      "type_id": 14,
      "name": "卡",
      "symbol": "cal",
      "description": null
    },
    {
      "id": 72,
      "type_id": 14,
      "name": "千瓦时",
      "symbol": "kW.h",
      "description": null
    },
    {
      "id": 73,
      "type_id": 15,
      "name": "千瓦",
      "symbol": "kW",
      "description": null
    },
    {
      "id": 74,
      "type_id": 15,
      "name": "瓦",
      "symbol": "W",
      "description": null
    },
    {
      "id": 75,
      "type_id": 15,
      "name": "马力",
      "symbol": "hp",
      "description": null
    },
    {
      "id": 76,
      "type_id": 16,
      "name": "千米/小时",
      "symbol": "km/h",
      "description": null
    },
    {
      "id": 77,
      "type_id": 16,
      "name": "米/秒",
      "symbol": "m/s",
      "description": null
    },
    {
      "id": 78,
      "type_id": 16,
      "name": "节",
      "symbol": "kn",
      "description": null
    },
    {
      "id": 79,
      "type_id": 17,
      "name": "法",
      "symbol": "F",
      "description": null
    },
    {
      "id": 80,
      "type_id": 17,
      "name": "微法",
      "symbol": "mF",
      "description": null
    },
    {
      "id": 81,
      "type_id": 17,
      "name": "皮法",
      "symbol": "pF",
      "description": null
    },
    {
      "id": 82,
      "type_id": 18,
      "name": "欧姆",
      "symbol": "Ω",
      "description": null
    },
    {
      "id": 83,
      "type_id": 19,
      "name": "立方米/秒",
      "symbol": "(m3/s)",
      "description": null
    },
    {
      "id": 84,
      "type_id": 20,
      "name": "赫兹",
      "symbol": "Hz",
      "description": null
    },
    {
      "id": 85,
      "type_id": 21,
      "name": "库伦",
      "symbol": "C",
      "description": null
    },
    {
      "id": 86,
      "type_id": 22,
      "name": "伏特",
      "symbol": "V",
      "description": null
    },
    {
      "id": 87,
      "type_id": 23,
      "name": "西门子",
      "symbol": "S",
      "description": null
    },
    {
      "id": 88,
      "type_id": 24,
      "name": "韦伯",
      "symbol": "Wb",
      "description": null
    },
    {
      "id": 89,
      "type_id": 25,
      "name": "特斯拉",
      "symbol": "T",
      "description": null
    },
    {
      "id": 90,
      "type_id": 26,
      "name": "亨利",
      "symbol": "H",
      "description": null
    },
    {
      "id": 91,
      "type_id": 27,
      "name": "流明",
      "symbol": "lm",
      "description": null
    },
    {
      "id": 92,
      "type_id": 28,
      "name": "勒克斯",
      "symbol": "lx",
      "description": null
    },
    {
      "id": 93,
      "type_id": 29,
      "name": "贝可",
      "symbol": "Bq",
      "description": null
    },
    {
      "id": 94,
      "type_id": 30,
      "name": "戈瑞",
      "symbol": "Gy",
      "description": null
    },
    {
      "id": 95,
      "type_id": 31,
      "name": "希沃特",
      "symbol": "Sv",
      "description": null
    },
    {
      "id": 96,
      "type_id": 32,
      "name": "转/分",
      "symbol": "r/min",
      "description": null
    },
    {
      "id": 97,
      "type_id": 33,
      "name": "电子伏",
      "symbol": "eV",
      "description": null
    },
    {
      "id": 98,
      "type_id": 34,
      "name": "分贝",
      "symbol": "dB",
      "description": null
    },
    {
      "id": 99,
      "type_id": 35,
      "name": "特克斯",
      "symbol": "tex",
      "description": null
    }
  ];

  const organizations = [
    {
      id: 1,
      "编号": null,
      "名称": null,
      "类型": 0,
      "地址": null,
      "电话": null,
      "联系人": null,
      "管理员id": 1,
      "备注": null
    }
  ];

  const users = [
    {
      id: 1,
      "登录名": "admin",
      "姓名": null,
      "性别": 0,
      "生日": null,
      "组织id": 1,
      "住址": null,
      "电话": null,
      email: null,
      website: null,
      "密码": "admin123",
      role: "管理员",
      "备注": null
    }
  ];

  const infoConfigs = [];
  const files = [];
  const models = [];
  const states = [];
  const batchs = [];
  const baseLines = [];
  const airs = [];
  const configurations = [];
  const softwares = [];

  const supportConfigs = [
    {
      "id": 1,
      "name": "随机设备",
      "parent_id": null,
      "table_no": null,
      "template_id": null
    },
    {
      "id": 2,
      "name": "随机工具",
      "parent_id": null,
      "table_no": null,
      "template_id": null
    },
    {
      "id": 3,
      "name": "随机备件",
      "parent_id": null,
      "table_no": null,
      "template_id": null
    },
    {
      "id": 4,
      "name": "随机资料",
      "parent_id": null,
      "table_no": null,
      "template_id": null
    },
    {
      "id": 5,
      "name": "一线保障装设备",
      "parent_id": 1,
      "table_no": 74,
      "template_id": 1
    },
    {
      "id": 6,
      "name": "二线保障设备",
      "parent_id": 1,
      "table_no": 74,
      "template_id": 1
    },
    {
      "id": 7,
      "name": "随机工具",
      "parent_id": 2,
      "table_no": 74,
      "template_id": 1
    },
    {
      "id": 8,
      "name": "大比例随机备件",
      "parent_id": 3,
      "table_no": 74,
      "template_id": 1
    },
    {
      "id": 9,
      "name": "挂架和导发装置",
      "parent_id": 3,
      "table_no": 74,
      "template_id": 1
    },
    {
      "id": 10,
      "name": "机械专业随机备件目录",
      "parent_id": 3,
      "table_no": 75,
      "template_id": 1
    },
    {
      "id": 11,
      "name": "随机资料目录",
      "parent_id": 4,
      "table_no": 76,
      "template_id": 1
    },
    {
      "id": 12,
      "name": "随机资料配套目录",
      "parent_id": 4,
      "table_no": 77,
      "template_id": 1
    }
  ];

  const auxiliaries = [];
  const machineAux = [];
  const resourceAux = [];
  const resourceDirectoryAux = [];
  const technologyStateFiles = [];
  const lifespans = [];
  const doubleCurrents = [];
  const problems = [];
  const changeHistories = [];
  const changeHistoryDetails = [];

  const ids = {
    templates: 2,
    types: 65537,
    codes: 48,
    schemas: 261,
    lists: 24,
    unit_types: 36,
    units: 100,
    '组织': 2,
    '用户': 2,
    info_configs: 1,
    '机型': 1,
    '状态': 1,
    files : 1,
    '批次': 1,
    '基线': 1,
    '飞机': 1,
    '构型': 1,
    softwares: 1,
    support_configs: 13,
    '随机设备工具': 1,
    '机械专业随机备件': 1,
    '随机资料目录': 1,
    '随机资料配套目录': 1,
    '技术状态文件': 1,
    '有寿件': 1,
    '双流水': 1,
    '交付遗留问题及处置情况': 1,
    change_histories: 1,
    change_history_details: 1
  };

  const getNewId = function (tableName) {
    const result = ids[tableName];
    ++ids[tableName];
    return result
  };

  const filter = (collection, conditions) => {
    let result = collection;
    const conditionCount = conditions.length;
    for (let i = 0; i < conditionCount; ++i) {
      if (conditions[i] !== '') {
        const operators = conditions[i].match(/!=|<=|>=|=|>|<| LIKE | is /g);
        if (!operators) {
          continue;
        }
        let operator = operators[0];
        const pair = conditions[i].split(operator);
        let name = pair[0].trim();
        if (name[0] === '"' && name[name.length - 1] === '"') {
          name = name.substring(1, name.length - 1);
        }
        let value = pair[1].trim();
        if (value[0] == '\'' && value[value.length - 1] == '\'') {
          value = value.substring(1, value.length - 1);
        }
        result = result.filter(item => {
          const objectValue = item[name];
          let r = false;
          switch (operator) {
            case '=':
              r = String(objectValue) == value;
              break
            case '!=':
              r = String(objectValue) != value;
              break
            case '<':
              r = Number(objectValue) < Number(value);
              break
            case '>':
              r = Number(objectValue) > Number(value);
              break
            case '<=':
              r = Number(objectValue) <= Number(value);
              break
            case '>=':
              r = Number(objectValue) >= Number(value);
              break
            case ' is ':
              r = objectValue === null;
              break
            case ' LIKE ':
              if (value.length) {
                const originValue = String(objectValue);
                if (value[0] == '%') {
                  if (value[value.length - 1] == '%') {
                    r = originValue.includes(value.substring(1, value.length - 1));
                  } else {
                    r = originValue.endsWith(value.substring(1));
                  }
                } else {
                  if (value[value.length - 1] == '%') {
                    r = originValue.startsWith(value.substring(0, value.length - 1));
                  } else {
                    r = originValue == value;
                  }
                }
              }
              break
          }
          return r
        });
      }
    }
    return result
  };

  const orderBy = (collection, orders) => {
    // FIXME
    if (orders.length > 0) {
      const firstOrder = orders[0];
      if (firstOrder !== '') {
        const p = firstOrder.split(' ');
        let column = firstOrder;
        let direction = 'ASC';
        if (p.length > 1) {
          column = p[0].trim();
          direction = p[1].trim().toUpperCase();
        }
        if (column[0] === '"' && column[column.length - 1] === '"') {
          column = column.substring(1, column.length - 1);
        }
        collection.sort((a, b) => {
          let r = a[column] - b[column];
          if (direction == 'DESC') {
            r = -r;
          }
          return r
        });
      }
    }
  };

  const Select = (tableName, whereCause, order) => {
    let result;
    if (typeof db_select == 'function') {
      //dp(`SELECT * FROM ${tableName} WHERE ${whereCause} ORDER BY ${order};`)
      result = db_select(tableName, whereCause, order);
      return JSON.parse(result.replace(/\n/g,"\\n").replace(/\r/g,"\\r"))
    } else {
      result = [];
      const conditions = whereCause.split(' AND ');
      const orders = order.split(',');
      switch (tableName) {
        case 'templates':
          result = filter(templates, conditions);
          break
        case 'types':
          result = filter(types, conditions);
          break
        case 'codes':
          result = filter(codes, conditions);
          break
        case 'schemas':
          result = filter(schemas, conditions);
          break
        case 'lists':
          result = filter(lists, conditions);
          break
        case 'unit_types':
          result = filter(unitTypes, conditions);
          break
        case 'units':
          result = filter(units, conditions);
          break
        case '组织':
          result = filter(organizations, conditions);
          break
        case '用户':
          result = filter(users, conditions);
          break
        case 'info_configs':
          result = filter(infoConfigs, conditions);
          break
        case '机型':
          result = filter(models, conditions);
          break
        case '状态':
          result = filter(states, conditions);
          break
        case 'files':
          result = filter(files, conditions);
          break
        case '批次':
          result = filter(batchs, conditions);
          break
        case '基线':
          result = filter(baseLines, conditions);
          break
        case '飞机':
          result = filter(airs, conditions);
          break
        case '构型':
          result = filter(configurations, conditions);
          break
        case 'softwares':
          result = filter(softwares, conditions);
          break
        case 'support_configs':
          result = filter(supportConfigs, conditions);
          break
        case '随机设备工具':
          result = filter(auxiliaries, conditions);
          break
        case '机械专业随机备件':
          result = filter(machineAux, conditions);
          break
        case '随机资料目录':
          result = filter(resourceAux, conditions);
          break
        case '随机资料配套目录':
          result = filter(resourceDirectoryAux, conditions);
          break
        case '技术状态文件':
          result = filter(technologyStateFiles, conditions);
          break
        case '有寿件':
          result = filter(lifespans, conditions);
          break
        case '双流水':
          result = filter(doubleCurrents, conditions);
          break
        case '交付遗留问题及处置情况':
          result = filter(problems, conditions);
          break
        case 'change_histories':
          result = filter(changeHistories, conditions);
          break
        case 'change_history_details':
          result = filter(changeHistoryDetails, conditions);
          break
      }
      orderBy(result, orders);
      return extendDeep([], result, true)
    }
  };

  const Insert = (tableName, data) => {
    if (typeof db_insert == 'function') {
      //dp(`INSERT INTO ${tableName} (...) VALUES (${JSON.stringify(data)});`)
      return db_insert(tableName, JSON.stringify(data))
    } else {
      if (!data.id) {
        data.id = getNewId(tableName);
      }
      data = extendDeep({},data);
      switch (tableName) {
        case 'templates':
          templates.push(data);
          break
        case 'types':
          types.push(data);
          break
        case 'codes':
          codes.push(data);
          break
        case 'schemas':
          schemas.push(data);
          break
        case 'lists':
          lists.push(data);
          break
        case 'unit_types':
          unitTypes.push(data);
          break
        case 'units':
          units.push(data);
          break
        case '组织':
          organizations.push(data);
          break
        case '用户':
          users.push(data);
          break
        case 'info_configs':
          infoConfigs.push(data);
          break
        case '机型':
          models.push(data);
          break
        case '状态':
          states.push(data);
          break
        case '批次':
          batchs.push(data);
          break
        case 'files':
          files.push(data);
          break
        case '基线':
          baseLines.push(data);
          break
        case '飞机':
          airs.push(data);
          break
        case '构型':
          configurations.push(data);
          break
        case 'softwares':
          softwares.push(data);
          break
        case 'support_configs':
          supportConfigs.push(data);
          break
        case '随机设备工具':
          auxiliaries.push(data);
          break
        case '机械专业随机备件':
          machineAux.push(data);
          break
        case '随机资料目录':
          resourceAux.push(data);
          break
        case '随机资料配套目录':
          resourceDirectoryAux.push(data);
          break
        case '技术状态文件':
          technologyStateFiles.push(data);
          break
        case '有寿件':
          lifespans.push(data);
          break
        case '双流水':
          doubleCurrents.push(data);
          break
        case '交付遗留问题及处置情况':
          problems.push(data);
          break
        case 'change_histories':
          changeHistories.push(data);
          break
        case 'change_history_details':
          changeHistoryDetails.push(data);
          break
      }
      return data.id
    }
  };

  const Update = (tableName, data) => {
    if (typeof db_update == 'function') {
      //dp(`UPDATE ${tableName} SET ... = ${JSON.stringify(data)};`)
      db_update(tableName, JSON.stringify(data));
    } else {
      let theData = null;
      switch (tableName) {
        case 'templates':
          theData = templates.find(item => item.id == data.id);
          break
        case 'types':
          theData = types.find(item => item.id == data.id);
          break
        case 'codes':
          theData = codes.find(item => item.id == data.id);
          break
        case 'schemas':
          theData = schemas.find(item => item.id == data.id);
          break
        case 'lists':
          theData = lists.find(item => item.id == data.id);
          break
        case 'unit_types':
          theData = unitTypes.find(item => item.id == data.id);
          break
        case 'units':
          theData = units.find(item => item.id == data.id);
          break
        case '组织':
          theData = organizations.find(item => item.id == data.id);
          break
        case '用户':
          theData = users.find(item => item.id == data.id);
          break
        case 'info_configs':
          theData = infoConfigs.find(item => item.id == data.id);
          break
        case '机型':
          theData = models.find(item => item.id == data.id);
          break
        case '状态':
          theData = states.find(item => item.id == data.id);
          break
        case '批次':
          theData = batchs.find(item => item.id == data.id);
          break
        case 'files':
          theData = files.find(item => item.id == data.id);
          break
        case '基线':
          theData = baseLines.find(item => item.id == data.id);
          break
        case '飞机':
          theData = airs.find(item => item.id == data.id);
          break
        case '构型':
          theData = configurations.find(item => item.id == data.id);
          break
        case 'softwares':
          theData = softwares.find(item => item.id == data.id);
          break
        case 'support_configs':
          theData = supportConfigs.find(item => item.id == data.id);
          break
        case '随机设备工具':
          theData = auxiliaries.find(item => item.id == data.id);
          break
        case '机械专业随机备件':
          theData = machineAux.find(item => item.id == data.id);
          break
        case '随机资料目录':
          theData = resourceAux.find(item => item.id == data.id);
          break
        case '随机资料配套目录':
          theData = resourceDirectoryAux.find(item => item.id == data.id);
          break
        case '技术状态文件':
          theData = technologyStateFiles.find(item => item.id == data.id);
          break
        case '有寿件':
          theData = lifespans.find(item => item.id == data.id);
          break
        case '双流水':
          theData = doubleCurrents.find(item => item.id == data.id);
          break
        case '交付遗留问题及处置情况':
          theData = problems.find(item => item.id == data.id);
          break
        case 'change_histories':
          theData = changeHistories.find(item => item.id == data.id);
          break
        case 'change_history_details':
          theData = changeHistoryDetails.find(item => item.id == data.id);
          break
      }
      if (theData) {
        for (const propertyName in data) {
          // if (data.hasOwnProperty(propertyName)) {
          theData[propertyName] = data[propertyName];
          // }
        }
      }
    }
  };

  const FillFileContent = (tableName, id, columnName, data) => {
    if (typeof db_fillFileContent == 'function') {
      //dp(`UPDATE ${tableName} SET ${columnName} = ${data} WHERE id = ${id};`)
      db_fillFileContent(tableName, id, columnName, data);
    } else {
      let theData = null;
      switch (tableName) {
        case 'templates':
          theData = templates.find(item => item.id == id);
          break
        case 'types':
          theData = types.find(item => item.id == id);
          break
        case 'codes':
          theData = codes.find(item => item.id == id);
          break
        case 'schemas':
          theData = schemas.find(item => item.id == id);
          break
        case 'lists':
          theData = lists.find(item => item.id == id);
          break
        case 'unit_types':
          theData = unitTypes.find(item => item.id == id);
          break
        case 'units':
          theData = units.find(item => item.id == id);
          break
        case '组织':
          theData = organizations.find(item => item.id == id);
          break
        case '用户':
          theData = users.find(item => item.id == id);
          break
        case '机型':
          theData = models.find(item => item.id == id);
          break
        case '状态':
          theData = states.find(item => item.id == id);
          break
        case '批次':
          theData = batchs.find(item => item.id == id);
          break
        case 'files':
          theData = files.find(item => item.id == id);
          break
        case '基线':
          theData = baseLines.find(item => item.id == id);
          break
        case '飞机':
          theData = airs.find(item => item.id == id);
          break
        case '构型':
          theData = configurations.find(item => item.id == id);
          break
        case 'support_configs':
          theData = supportConfigs.find(item => item.id == id);
          break
        case '随机设备工具':
          theData = auxiliaries.find(item => item.id == id);
          break
        case '机械专业随机备件':
          theData = machineAux.find(item => item.id == id);
          break
        case '随机资料目录':
          theData = resourceAux.find(item => item.id == id);
          break
        case '随机资料配套目录':
          theData = resourceDirectoryAux.find(item => item.id == id);
          break
        case '技术状态文件':
          theData = technologyStateFiles.find(item => item.id == id);
          theData["digest"] = md5__default['default'].hex_md5(data);
          break
        case '有寿件':
          theData = lifespans.find(item => item.id == id);
          break
        case '双流水':
          theData = doubleCurrents.find(item => item.id == id);
          break
        case '交付遗留问题及处置情况':
          theData = problems.find(item => item.id == id);
          break
        case 'change_histories':
          theData = changeHistories.find(item => item.id == id);
          break
        case 'change_history_details':
          theData = changeHistoryDetails.find(item => item.id == id);
          break
      }
      if (theData) {
        theData[columnName] = data;
      }
    }
  };

  const GetFileContent = (tableName, id, columnName) => {
    if (typeof db_getFileContent == 'function') {
      //dp(`SELECT ${columnName} FROM ${tableName} WHERE id = ${id};`)
      return db_getFileContent(tableName, id, columnName)
    } else {
      let theData = null;
      switch (tableName) {
        case 'templates':
          theData = templates.find(item => item.id == id);
          break
        case 'types':
          theData = types.find(item => item.id == id);
          break
        case 'codes':
          theData = codes.find(item => item.id == id);
          break
        case 'schemas':
          theData = schemas.find(item => item.id == id);
          break
        case 'lists':
          theData = lists.find(item => item.id == id);
          break
        case 'unit_types':
          theData = unitTypes.find(item => item.id == id);
          break
        case 'units':
          theData = units.find(item => item.id == id);
          break
        case '组织':
          theData = organizations.find(item => item.id == id);
          break
        case '用户':
          theData = users.find(item => item.id == id);
          break
        case '机型':
          theData = models.find(item => item.id == id);
          break
        case '状态':
          theData = states.find(item => item.id == id);
          break
        case '批次':
          theData = batchs.find(item => item.id == id);
          break
        case 'files':
          theData = files.find(item => item.id == id);
          break
        case '基线':
          theData = baseLines.find(item => item.id == id);
          break
        case '飞机':
          theData = airs.find(item => item.id == id);
          break
        case '构型':
          theData = configurations.find(item => item.id == id);
          break
        case 'support_configs':
          theData = supportConfigs.find(item => item.id == id);
          break
        case '随机设备工具':
          theData = auxiliaries.find(item => item.id == id);
          break
        case '机械专业随机备件':
          theData = machineAux.find(item => item.id == id);
          break
        case '随机资料目录':
          theData = resourceAux.find(item => item.id == id);
          break
        case '随机资料配套目录':
          theData = resourceDirectoryAux.find(item => item.id == id);
          break
        case '技术状态文件':
          theData = technologyStateFiles.find(item => item.id == id);
          // theData["digest"] = md5.hex_md5(data)
          break
        case '有寿件':
          theData = lifespans.find(item => item.id == id);
          break
        case '双流水':
          theData = doubleCurrents.find(item => item.id == id);
          break
        case '交付遗留问题及处置情况':
          theData = problems.find(item => item.id == id);
          break
        case 'change_histories':
          theData = changeHistories.find(item => item.id == id);
          break
        case 'change_history_details':
          theData = changeHistoryDetails.find(item => item.id == id);
          break
      }
      if (theData) {
        return theData[columnName]
      }
    }
  };

  const OpenFile = id => {
    if (typeof db_openfile === 'function') {
      db_openfile(id);
    } else {
      console.log('not support');
    }
  };

  const Remove = (tableName, id) => {
    if (typeof db_remove == 'function') {
      //dp(`DELETE FROM ${tableName} WHERE id = ${id}`)
      db_remove(tableName, id);
    } else {
      let index = -1;
      switch (tableName) {
        case 'templates':
          index = templates.findIndex(item => item.id == id);
          if (index !== -1) {
            templates.splice(index, 1);
          }
          break
        case 'types':
          index = types.findIndex(item => item.id == id);
          if (index !== -1) {
            types.splice(index, 1);
          }
          break
        case 'codes':
          index = codes.findIndex(item => item.id == id);
          if (index !== -1) {
            codes.splice(index, 1);
          }
          break
        case 'schemas':
          index = schemas.findIndex(item => item.id == id);
          if (index !== -1) {
            schemas.splice(index, 1);
          }
          break
        case 'lists':
          index = lists.findIndex(item => item.id == id);
          if (index !== -1) {
            lists.splice(index, 1);
          }
          break
        case 'unit_types':
          index = unitTypes.findIndex(item => item.id == id);
          if (index !== -1) {
            unitTypes.splice(index, 1);
          }
          break
        case 'units':
          index = units.findIndex(item => item.id == id);
          if (index !== -1) {
            units.splice(index, 1);
          }
          break
        case '组织':
          index = organizations.findIndex(item => item.id == id);
          if (index !== -1) {
            organizations.splice(index, 1);
          }
          break
        case '用户':
          index = users.findIndex(item => item.id == id);
          if (index !== -1) {
            users.splice(index, 1);
          }
          break
        case 'info_configs':
          index = infoConfigs.findIndex(item => item.id == data.id);
          if (index !== -1) {
            infoConfigs.splice(index, 1);
          }
          break
        case '机型':
          index = models.findIndex(item => item.id == id);
          if (index !== -1) {
            models.splice(index, 1);
          }
          break
        case '状态':
          index = states.findIndex(item => item.id == id);
          if (index !== -1) {
            states.splice(index, 1);
          }
          break
        case '批次':
          index = batchs.findIndex(item => item.id == id);
          if (index !== -1) {
            batchs.splice(index, 1);
          }
          break
        case 'files':
          index = files.findIndex(item => item.id == id);
          if (index !== -1) {
            files.splice(index, 1);
          }
          break
        case '基线':
          index = baseLines.findIndex(item => item.id == id);
          if (index !== -1) {
            baseLines.splice(index, 1);
          }
          break
        case '飞机':
          index = airs.findIndex(item => item.id == id);
          if (index !== -1) {
            airs.splice(index, 1);
          }
          break
        case '构型':
          index = configurations.findIndex(item => item.id == id);
          if (index !== -1) {
            configurations.splice(index, 1);
          }
          break
        case 'softwares':
          index = softwares.findIndex(item => item.id == data.id);
          if (index !== -1) {
            softwares.splice(index, 1);
          }
          break
        case 'support_configs':
          index = supportConfigs.findIndex(item => item.id == id);
          if (index !== -1) {
            supportConfigs.splice(index, 1);
          }
          break
        case '随机设备工具':
          index = auxiliaries.findIndex(item => item.id == id);
          if (index !== -1) {
            auxiliaries.splice(index, 1);
          }
          break
        case '机械专业随机备件':
          index = machineAux.findIndex(item => item.id == id);
          if (index !== -1) {
            machineAux.splice(index, 1);
          }
          break
        case '随机资料目录':
          index = resourceAux.findIndex(item => item.id == id);
          if (index !== -1) {
            resourceAux.splice(index, 1);
          }
          break
        case '随机资料配套目录':
          index = resourceDirectoryAux.findIndex(item => item.id == id);
          if (index !== -1) {
            resourceDirectoryAux.splice(index, 1);
          }
          break
        case '技术状态文件':
          index = technologyStateFiles.findIndex(item => item.id == id);
          if (index !== -1) {
            technologyStateFiles.splice(index, 1);
          }
          break
        case '有寿件':
          index = lifespans.findIndex(item => item.id == id);
          if (index !== -1) {
            lifespans.splice(index, 1);
          }
          break
        case '双流水':
          index = doubleCurrents.findIndex(item => item.id == id);
          if (index !== -1) {
            doubleCurrents.splice(index, 1);
          }
          break
        case '交付遗留问题及处置情况':
          index = problems.findIndex(item => item.id == id);
          if (index !== -1) {
            problems.splice(index, 1);
          }
          break
        case 'change_histories':
          index = changeHistories.findIndex(item => item.id == id);
          if (index !== -1) {
            changeHistories.splice(index, 1);
          }
          break
        case 'change_history_details':
          index = changeHistoryDetails.findIndex(item => item.id == id);
          if (index !== -1) {
            changeHistoryDetails.splice(index, 1);
          }
          break
      }
    }
  };

  const selectOne = (tableName, whereCause) => {
    let result = null;
    const r = Select(tableName, whereCause, '');
    if (r && r.length) {
      result = r[0];
    }
    return result
  };

  const getTypeById = id => {
    return selectOne('types', `id = ${id}`)
  };

  const getBatchById = id => {
    return selectOne('批次', `id = ${id}`)
  };

  const getModelById = id => {
    return selectOne('机型', `id = ${id}`)
  };

  const getStatesById = id => {
    return selectOne('状态', `id = ${id}`)
  };

  const getTypeByTemplateIdAndNo = (templateId, no) => {
    let result = selectOne('types', `template_id = ${templateId} AND no = ${no}`);
    if (!result) {
      result = selectOne('types', `template_id is null AND no = ${no}`);
    }
    return result
  };

  const getTypeDetailTableByNumber = n => {
    let result = '';
    switch (n) {
      case 1:
        result = 'codes';
        break
      case 2:
        result = 'schemas';
        break
      case 3:
        result = 'lists';
        break
    }
    return result
  };

  const getCodeValues = (templateId, codeNo) => {
    let r = Select('codes', `template_id = ${templateId} AND parent_no = ${codeNo}`, '');
    if (!r || !r.length) {
      r = Select('codes', `template_id is null AND parent_no = ${codeNo}`, '');
    }
    r.sort((a, b) => Number.parseInt(a.no) - Number.parseInt(b.no));
    return r
  };

  const getSchemaComponents = (templateId, schemaNo) => {
    let r = Select('schemas', `template_id = ${templateId} AND parent_no = ${schemaNo}`, '');
    if (!r || r.length === 0) {
      r = Select('schemas', `template_id is null AND parent_no = ${schemaNo}`, '');
    }
    r.forEach(item => {
        if (!item.restriction || typeof item.restriction === 'string') {
            item.restriction = extendDeep({
                unit: {},
                min: '',
                max: '',
                precision: '',
                format: ''
            }, isJSON(item.restriction) ? JSON.parse(item.restriction) : {});
        }
    });
    r.sort((a, b) => Number.parseInt(a.no) - Number.parseInt(b.no));
    return r
  };

  const getListElement = (templateId, listNo) => {
    let r = selectOne('lists', `template_id = ${templateId} AND parent_no = ${listNo}`);
    if (!r) {
      r = selectOne('lists', `template_id is null AND parent_no = ${listNo}`);
    }
    return r
  };

  const getTableSchema = (templateId, listNo) => {
    const result = getListElement(templateId, listNo);
    result.element_type = getTypeByTemplateIdAndNo(templateId, result.element_type_no);
    result.element_type.children = getSchemaComponents(templateId, result.element_type_no);
    return result
  };

  const groupRows = (rows, groupColumn) => {
    const groupNames = {};
    rows.forEach(row => {
      const groupName = row[groupColumn];
      // if (!groupNames.hasOwnProperty(groupName)) {
      if (!(groupName in groupNames)) {
        groupNames[groupName] = [];
      }
      groupNames[groupName].push(row);
    });
    return groupNames
  };

  const simpleClone = function simpleClone (origin) {
    const assign = function (obj, name, value) {
      if (typeof value === 'object' && value !== null) {
        obj[name] = simpleClone(value);
      } else {
        obj[name] = value;
      }
    };
    let target;
    if (typeof origin === 'object') {
      if (Array.isArray(origin)) {
        target = [];
        origin.forEach((item, index) => {
          assign(target, index, item);
        });
      } else {
        target = {};
        Object.entries(origin).forEach(item => {
          assign(target, item[0], item[1]);
        });
      }
    } else {
      target = origin;
    }
    return target
  };

  const extendDeep = function extendDeep(target, origin, isArray = false) {
    const assign = function (obj, name, value) {
      if (typeof value === 'object' && value !== null) {
        const isArr = Array.isArray(value);
        obj[name] = extendDeep(isArr ? [] : {}, value, isArr);
      } else {
        obj[name] = value;
      }
    };
    if (isArray) {
      origin.forEach((item, index) => {
        assign(target, index, item);
      });
    } else {
      Object.entries(origin).forEach(item => {
        assign(target, item[0], item[1]);
      });
    }
    return target
  };

  const cloneTemplate = (oldTemplateId, templateName, templateDescription) => {
    // return new Promise((resolve, reject) => {
      const template = {
        name: templateName,
        description: templateDescription,
        state: 0
      };
      const templateId = Insert('templates', template);
    
      const oldTypes = Select('types', `template_id = ${oldTemplateId}`, 'id ASC');
      oldTypes.forEach(oldType => {
        /*{
          id: `${getNewId('types')}`,
          no: oldType.no,
          template_id: `${templateId}`,
          name: oldType.name,
          type: oldType.type,
          description: oldType.description
        }*/
        const type = extendDeep({}, oldType);
        delete type.id;
        type.template_id = templateId;
        Insert('types', type);
        const detailTable = getTypeDetailTableByNumber(Number.parseInt(type.type));
        const oldDetails = Select(detailTable, `template_id = ${oldTemplateId} AND parent_no = ${type.no}`, detailTable === 'lists'?'':'no ASC');
        let command = `INSERT INTO ${detailTable} (template_id, parent_id, name, table_no) VALUES `;
        switch (type.type) {
          case 1:
            command = 'INSERT INTO codes (template_id, parent_no, no, value, description) VALUES ';
            break
          case 2:
            command = 'INSERT INTO schemas (template_id, parent_no, no, name, type_no, is_required, restriction, description) VALUES ';
            break
          case 3:
            command = 'INSERT INTO lists (template_id, parent_no, element_type_no, min, max, group_by, is_hidden, description) VALUES ';
            break
        }
        let isFirst = true;
        oldDetails.forEach(oldDetail => {
          if (isFirst) {
            isFirst = false;
          } else {
            command += ', ';
          }
          const sqlQuoteString = v => v ? "'" + v + "'" : 'NULL';
          switch (type.type) {
            case 1:
              command += `(${templateId}, ${oldDetail.parent_no}, ${oldDetail.no}, '${oldDetail.value}', ${sqlQuoteString(oldDetail.description)})`;
              break
            case 2:
              command += `(${templateId}, ${oldDetail.parent_no}, ${oldDetail.no}, '${oldDetail.name}', ${oldDetail.type_no}, ${oldDetail.is_required}, ${sqlQuoteString(oldDetail.restriction)}, ${sqlQuoteString(oldDetail.description)})`;
              // command += `(${templateId}, ${oldDetail.parent_no}, ${oldDetail.no}, '${oldDetail.name}', ${oldDetail.type_no}, ${oldDetail.is_required}, ${sqlQuoteString(oldDetail.restriction)}, ${sqlQuoteString(oldDetail.description)})`
              break
            case 3:
              command += `(${templateId}, ${oldDetail.parent_no}, ${oldDetail.element_type_no}, ${oldDetail.min}, ${oldDetail.max}, ${sqlQuoteString(oldDetail.group_by)}, ${oldDetail.is_hidden}, ${sqlQuoteString(oldDetail.description)})`;
              break
          }
          // if (typeof db_executeSql !== 'function') {
            const detail = extendDeep({}, oldDetail);
            // detail.id = `${getNewId(detailTable)}`
            delete detail.id;
            detail.template_id = templateId;
            Insert(detailTable, detail);
          // }
        });
      });
    
      let command = 'INSERT INTO support_configs (template_id, parent_id, name, table_no) VALUES ';
      let isFirst = true;
      const oldSupportConfigs = Select('support_configs', `template_id = ${oldTemplateId}`, '');
      oldSupportConfigs.forEach(oldSupportConfig => {
        if (isFirst) {
          isFirst = false;
        } else {
          command += ", ";
        }
        command += `(${templateId}, ${oldSupportConfig.parent_id}, '${oldSupportConfig.name}', ${oldSupportConfig.table_no})`;
        if (typeof db_executeSql !== 'function') {
          const supportConfig = extendDeep({}, oldSupportConfig);
          delete supportConfig.id;
          supportConfig.template_id = templateId;
          Insert('support_configs', supportConfig);
        }
      });
      if (typeof db_executeSql === 'function') {
        db_executeSql(command);
      }
    //   resolve(templateId)
    // })

    return templateId
  };

  const cloneBaseLine = (oldBaseLineId, objectType, objectId, description) => {
    const baseline = {
      '适用目标': objectType,
      '目标id': objectId,
      '备注': description
    };
    const baseLineId = Insert('基线', baseline);

    const oldConfigurations = Select('构型', `基线id = ${oldBaseLineId}`, 'id ASC');
    const configurationIdMaps = {};
    oldConfigurations.forEach(oldConfiguration => {
      /*
      {
        id,
        "基线id",
        parent_id,
        "编号",
        "序号",
        "名称",
        "型号",
        "单机定额",
        "装机部位",
        "承制单位id",
        "重要度",
        "状态",
        "主管",
        "版本",
        extended,
        "备注"
      }
      */
      const configuration = extendDeep({}, oldConfiguration);
      delete configuration.id;
      configuration['基线id'] = baseLineId;
      if(oldConfiguration.parent_id !== null){
        configuration.parent_id = configurationIdMaps[oldConfiguration.parent_id];
      }
      configurationIdMaps[oldConfiguration.id] = Insert('构型', configuration);
    });

    const oldAuxiliaries = Select('随机设备工具', `基线id = ${oldBaseLineId}`, 'id ASC');
    const auxiliaryIdMaps = {};
    oldAuxiliaries.forEach(oldAuxiliary => {
      /*
      {
        id,
        "基线id",
        parent_id,
        "专业",
        "序号",
        "名称",
        "型号",
        "用途",
        "配套比例",
        "定价",
        "承制单位id",
        extended,
        "备注"
      }
      */
      const auxiliary = extendDeep({}, oldAuxiliary);
      delete auxiliary.id;
      auxiliary['基线id'] = baseLineId;
      auxiliary.parent_id = auxiliaryIdMaps[oldAuxiliary.parentId];
      auxiliaryIdMaps[oldAuxiliary.id] = Insert('随机设备工具', auxiliary);
    });

    const oldMachineAuxes = Select('机械专业随机备件', `基线id = ${oldBaseLineId}`, 'id ASC');
    const machineAuxIdMaps = {};
    oldMachineAuxes.forEach(oldMachineAux => {
      /*
      {
        id,
        "基线id",
        parent_id,
        "序号",
        "名称",
        "型号(图号)",
        "单机数量",
        "比例及数量",
        "所属系统",
        "安装使用部位",
        "承制单位id",
        extended,
        "备注"
      }
      */
      const auxiliary = extendDeep({}, oldMachineAux);
      delete auxiliary.id;
      auxiliary['基线id'] = baseLineId;
      auxiliary.parent_id = machineAuxIdMaps[oldMachineAux.parentId];
      machineAuxIdMaps[oldMachineAux.id] = Insert('机械专业随机备件', auxiliary);
    });

    const oldResourceAuxes = Select('随机资料目录', `基线id = ${oldBaseLineId}`, 'id ASC');
    const resourceAuxIdMaps = {};
    oldResourceAuxes.forEach(oldResourceAux => {
      /*
      {
        id,
        "基线id",
        parent_id,
        "序号",
        "资料编号",
        "手册名称",
        "册数",
        "单册页数",
        "密级",
        extended,
        "备注"
      }
      */
      const auxiliary = extendDeep({}, oldResourceAux);
      delete auxiliary.id;
      auxiliary['基线id'] = baseLineId;
      auxiliary.parent_id = resourceAuxIdMaps[oldResourceAux.parentId];
      resourceAuxIdMaps[oldResourceAux.id] = Insert('随机资料目录', auxiliary);
    });

    const oldResourceDirectoryAuxes = Select('随机资料配套目录', `基线id = ${oldBaseLineId}`, 'id ASC');
    const resourceDirectoryAuxIdMaps = {};
    oldResourceDirectoryAuxes.forEach(oldResourceDirectoryAux => {
      /*
      {
        id,
        "基线id",
        parent_id,
        "序号",
        "资料名称",
        "手册编制的参考标准",
        "主要用途/使用时机/使用地点",
        "主要使用对象" text,
        extended,
        "备注"
      }
      */
      const auxiliary = extendDeep({}, oldResourceDirectoryAux);
      delete auxiliary.id;
      auxiliary['基线id'] = baseLineId;
      auxiliary.parent_id = resourceDirectoryAuxIdMaps[oldResourceDirectoryAux.parentId];
      resourceDirectoryAuxIdMaps[oldResourceDirectoryAux.id] = Insert('随机资料配套目录', auxiliary);
    });

    const oldTechStateFiles = Select('技术状态文件', `基线id = ${oldBaseLineId}`, 'id ASC');
    oldTechStateFiles.forEach(oldTechStateFile => {
      /*
      {
        id,
        "基线id",
        "类型",
        previous_version_id,
        path,
        "编号",
        "名称",
        "版本",
        "修订时间",
        "构型节点id",
        "附件",
        digest,
        state,
        extended,
        "备注"
      }
      */
      const techStateFile = extendDeep({}, oldTechStateFile);
      // delete techStateFile['附件']
      techStateFile['构型节点id'] =  configurationIdMaps[techStateFile['构型节点id']];
      delete techStateFile.id;
      techStateFile['基线id'] = baseLineId;
      Insert('技术状态文件', techStateFile);
    });

    const oldLifeSpans = Select('有寿件', `基线id = ${oldBaseLineId}`, 'id ASC');
    oldLifeSpans.forEach(oldLifeSpan => {
      /*
      {
        id,
        "基线id",
        extended,
        "备注"
      }
      */
      const lifeSpan = extendDeep({}, oldLifeSpan);
      delete lifeSpan.id;
      lifeSpan['基线id'] = baseLineId;
      Insert('有寿件', lifeSpan);
    });

    const oldDoubleCurrents = Select('双流水', `基线id = ${oldBaseLineId}`, 'id ASC');
    oldDoubleCurrents.forEach(oldDoubleCurrent => {
      /*
      {
        id,
        "基线id",
        extended,
        "备注"
      }
      */
      const doubleCurrent = extendDeep({}, oldDoubleCurrent);
      delete doubleCurrent.id;
      doubleCurrent['基线id'] = baseLineId;
      Insert('双流水', doubleCurrent);
    });

    const oldProblems = Select('交付遗留问题及处置情况', `基线id = ${oldBaseLineId}`, 'id ASC');
    oldProblems.forEach(oldProblem => {
      /*
      {
        id,
        "基线id",
        extended,
        "备注"
      }
      */
      const problem = extendDeep({}, oldProblem);
      delete problem.id;
      problem['基线id'] = baseLineId;
      Insert('交付遗留问题及处置情况', problem);
    });

    return baseLineId
  };

  const moveTechnologyStateFiles = (configurationId, newBaselineId) => {
    const files = Select('技术状态文件', `"构型节点id" = ${configurationId}`, '');
    for (const file of files) {
      file['基线id'] = newBaselineId;
      Update('技术状态文件', file);
    }
  };

  const moveConfigurationNode = (nodeId, newParentId) => {
    const move = (self, newBaseLineId) => {
      self['基线id'] = newBaseLineId;
      Update('构型', self);
      moveTechnologyStateFiles(self.id, newBaseLineId);
      const children = Select('构型', `parent_id = ${self.id}`, '');
      for (const child of children) {
        move(child, newBaseLineId);
      }
    };
    const self = selectOne('构型', `id = ${nodeId}`);
    self.parent_id = newParentId;
    const newParent = selectOne('构型', `id = ${newParentId}`);
    const newBaselineId = newParent['基线id'];
    move(self, newBaselineId);
  };

  const copyTechnologyStateFiles = (configurationId, newBaselineId, newConfigurationId) => {
    const files = Select('技术状态文件', `"构型节点id" = ${configurationId}`, '');
    for (const file of files) {
      file['基线id'] = newBaselineId;
      file['构型节点id'] = newConfigurationId;
      Insert('技术状态文件', file);
    }
  };

  const copyConfigurationNode = (nodeId, newParentId) => {
    const copy = (self, newBaseLineId, newParentId) => {
      const newSelf = extendDeep({}, self);
      delete newSelf.id;
      newSelf['基线id'] = newBaseLineId;
      newSelf.parent_id = newParentId;
      const newId = Insert('构型', newSelf);
      copyTechnologyStateFiles(self.id, newBaseLineId, newId);
      const children = Select('构型', `parent_id = ${self.id}`, '');
      for (const child of children) {
        copy(child, newBaseLineId, newId);
      }
    };
    const self = selectOne('构型', `id = ${nodeId}`);
    const newParent = selectOne('构型', `id = ${newParentId}`);
    const newBaselineId = newParent['基线id'];
    copy(self, newBaselineId, newParentId);
  };

  const removeTechnologyStateFiles = configurationId => {
    const files = Select('技术状态文件', `"构型节点id" = ${configurationId}`, '');
    for (const file of files) {
      Remove('技术状态文件', file.id);
    }
  };

  const removeConfigurationNode = nodeId => {
    const remove = nodeId => {
      const children = Select('构型', `parent_id = ${nodeId}`, '');
      for (const child of children) {
        remove(child.id);
      }
      Remove('构型', nodeId);
      removeTechnologyStateFiles(nodeId);
    };
    remove(nodeId);
  };

  const getTopTables = () => {
    const r = Select('types', 'template_id is null AND type = 3', '');
    r.sort((a, b) => Number.parseInt(a.id) - Number.parseInt(b.id));
    return r
  };

  const tables = getTopTables();

  const isTable = type => {
    return tables.find(item => item.id === type.id) !== undefined
  };

  const getTableTypeNo = tableName => {
    let result = 0;
    const table = tables.find(item => item.name === tableName);
    if (table) {
      result = table.no;
    }
    return result
  };

  const tableNos = {
    "组织": 65,
    "用户": 66,
    "机型": 67,
    "状态": 68,
    "批次": 69,
    "基线": 70,
    "飞机": 71,
    "构型": 72,
    support_configs: 73,
    "随机设备工具": 74,
    "机械专业随机备件目录": 75,
    "随机资料目录": 76,
    "随机资料配套目录": 77,
    "技术状态文件": 78,
    "有寿件": 79,
    "双流水": 80,
    "交付遗留问题及处置情况": 81,
    change_histories: 82,
    change_history_details: 83,
    info_configs: 84,
    softwares: 85
  };

  const construct = (templateId, parentNo, value) => {
    const result = [];
    const columns = getSchemaComponents(templateId, parentNo);
    columns.forEach(column => {
      result.push({
        name: column.name,
        relation: column,
        type: getTypeByTemplateIdAndNo(templateId, column.type_no),
        value: value[column.name]
      });
    });
    return result
  };

  const removeUnneedFields = (defines, fields) => {
    if (fields && fields.length) {
      for (let i = 0; i < defines.length; ++i) {
        if (fields.indexOf(defines[i].name) !== -1) {
          defines.splice(i, 1);
          --i;
        }
      }
    } else {
      for (let i = 0; i < defines.length; ++i) {
        if (defines[i].type.type === 3) {
          if (isTable(defines[i].type)) {
            defines.splice(i, 1);
            --i;
          }
        } else if (defines[i].name === 'id') {
          defines.splice(i, 1);
          --i;
        }
      }
    }
    return defines
  };

  const tableCaches = {};

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
            define.isForeignKey = true;
            define.title = define.name.replace('id', '');
            if (define.schema.description) {
              define.desc = JSON.parse(define.schema.description);
            }
            if (!Object.prototype.hasOwnProperty.call(tableCaches, define.type.name)) {
              tableCaches[define.type.name] = Select(define.type.name, '', '');
            }
            let r = Select(define.type.name, '', '');//tableCaches[define.type.name]
            if (define.desc) {
              if (define.desc.filter) {
                r = filter(r, define.desc.filter.split(' AND '));
              }
              if (define.desc.show) {
                r = r.map(item => ({
                  id: item.id,
                  value: item[define.desc.show]
                }));
              }
            }
            define.valueSet = r;
          }
        }
      }
    });
    return defines
  };

  const processEnums = defines => {
    defines.forEach(define => {
      if (define.type.type === 1) {
        define.children = getCodeValues(define.type.template_id, define.type.no);
      }
    });
    return defines
  };

  const processComposites = defines => {
    // const defineCount = defines.length
    for (let i = 0; i < defines.length; ++i) {
      if (defines[i].type.type === 2) {
        const children = construct(defines[i].type.template_id, defines[i].type.no, defines[i].value || {});
        if (children && children.length) {
          defines[i].children = children;
        } else {
          defines.splice(i, 1);
          --i;
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
  };

  const processTemplate = defines => {
    const defineCount = defines.length;
    for (let i = 0; i < defineCount; ++i) {
      const define = defines[i];
      if (define.name === '模板id') {
        if (define.value) {
          define.name = '模板';
          define.value = Select('templates', `id = ${define.value}`, '')[0].name;
          define.disabled = true;
        }
        break
      }
    }
    return defines
  };

  const processState = defines => {
      const defineCount = defines.length;
      for (let i = 0; i < defineCount; ++i) {
          if (defines[i].name === 'state') {
              defines.splice(i, 1);
              break
          }
      }
      return defines
  };

  const processId = defines => {
    const defineCount = defines.length;
    for (let i = 0; i < defineCount; ++i) {
      if (defines[i].name === 'id') {
        defines.splice(i, 1);
        break
      }
    }
    return defines
  };

  const getExtended = (templateId, tableName, value) => {
    let result = null;
    const tableNo = tableNos[tableName];
    if (tableNo) {
      const list = getListElement('null', tableNo);
      if (list) {
        const columns = getSchemaComponents('null', list.element_type_no);
        const extended = columns.find(column => column.name === 'extended');
        if (extended) {
          result = construct(templateId, extended.type_no, value);
        }
      }
    }
    return result
  };

  const getBatchExtended = (templateId, value) => {
    return getExtended(templateId, '批次', value)
  };

  const render = (templateId, parentNo, value) => {
    return processComposites(processForeignKey(processEnums(removeUnneedFields(construct(templateId, parentNo, value), ['id', '模板id', 'state']))))
    // return processComposites(processForeignKey(processEnums(processTemplate(processId(construct(templateId, parentNo, value))))))
  };

  const equals = (left, right) => {
    let result = true;
    const leftEntries = left.entries();
    const rightEntries = right.entries();
    if (leftEntries.length === rightEntries.length) {
      for (let i = 0; i < leftEntries.length; ++i) {
        const leftEntry = leftEntries[i];
        const rightEntry = rightEntries.find(entry => entry[0] === leftEntry[0]);
        if (rightEntry) {
          if (leftEntry[1] !== rightEntry[1]) {
            result = false;
            break
          }
        } else {
          result = false;
          break
        }
      }
    } else {
      result = false;
    }
    return result
  };

  const canOperation = (nodeId, newId) => {
    const childrenIncluded = (nodeId, newId) => {
      let result = false;
      const children = Select('构型', `parent_id = ${nodeId}`, '');
      for (const child of children) {
        if (child.id === newId) {
          result = true;
          break
        }
        result = childrenIncluded(child.id, newId);
      }
      return result
    };

    let result;
    if (nodeId === newId) {
      result = false;
    } else {
      result = !childrenIncluded(nodeId, newId);
    }
    return result
  };

  const compare = (templateId, batchs, airs) => {
    const compares = getTableSchema(templateId, tableNos['批次']);
    const result = [];
    // const baselines = []
    // const types = []
    compares.forEach(item => {
      const line = {
        name: item,
        isSame: true,
        values: []
      };
      switch (item) {
        case '使命任务':
          do {
            batchs.forEach(batch => {
              const state = selectOne('状态', `id = ${batch['状态id']}`);
              line.values.push(state['使命任务']);
            });
            airs.forEach(air => {
              const batch = selectOne('批次', `id = ${air.data['批次id']}`);
              const state = selectOne('状态', `id = ${batch['状态id']}`);
              line.values.push(state['使命任务']);
            });
            const value = line.values[0];
            for (let index = 1; index < line.values.length; ++index) {
              if (value !== line.values[index]) {
                line.isSame = false;
                break
              }
            }
            result.push(line);
          } while (false)
          break
        case '布局':
          do {
            batchs.forEach(batch => {
              line.values.push(JSON.parse(batch[item]));
            });
            airs.forEach(air => {
              const batch = selectOne('批次', `id = ${air['批次id']}`);
              line.values.push(JSON.parse(batch[item]));
            });
            result.push(line);
            const components = Select('schemas', `template_id = ${templateId} AND parent_no = 262`, 'no ASC');
            for (const component of components) {
              const subLine = {
                name: `${item}.${component.name}`,
                isSame: true,
                values: []
              };
              for (let i = 0; i < line.values.length; ++i) {
                subLine.values.push(line.values[i][component.name]); //getTypeByTemplateIdAndNo(templateId, component.type_no).name
              }
              const value = subLine.values[0];
              for (let index = 1; index < subLine.values.length; ++index) {
                if (value !== subLine.values[index]) {
                  subLine.isSame = false;
                  line.isSame = false;
                  break
                }
              }
              result.push(subLine);
            }
          } while (false)
          break
        case '主要技术参数':
          do {
            batchs.forEach(batch => {
              line.values.push(JSON.parse(batch[item]));
            });
            airs.forEach(air => {
              const batch = selectOne('批次', `id = ${air['批次id']}`);
              line.values.push(JSON.parse(batch[item]));
            });
            result.push(line);
            const components = Select('schemas', `template_id = ${templateId} AND parent_no = 263`, 'no ASC');
            for (const component of components) {
              const subLine = {
                name: `${item}.${component.name}`,
                isSame: true,
                values: []
              };
              for (let i = 0; i < line.values.length; ++i) {
                subLine.values.push(line.values[i][component.name]); //getTypeByTemplateIdAndNo(templateId, component.type_no).name
              }
              result.push(subLine);
              const subComponents = Select('schemas', `template_id = ${templateId} AND parent_no = ${component.type_no}`, 'no ASC');
              for (const subComponent of subComponents) {
                const subSubLine = {
                  name: `${item}.${component.name}.${subComponent.name}`,
                  isSame: true,
                  values: []
                };
                for (let j = 0; j < subLine.values.length; ++j) {
                  subSubLine.values.push(subLine.values[j][subComponent.name]);
                }
                const value = subSubLine.values[0];
                for (let index = 1; index < subSubLine.values.length; ++index) {
                  if (value !== subSubLine.values[index]) {
                    subSubLine.isSame = false;
                    subLine.isSame = false;
                    line.isSame = false;
                    break
                  }
                }
                result.push(subSubLine);
              }
            }
          } while (false)
          break
      }
    });
    return result
  };

  const exportTemplate = templateId => {
    if (typeof db_exportTemplatePackage === 'function') {
      return db_exportTemplatePackage(templateId)
    } else {
      console.log('not support');
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
  };

  const exportOrganizations = () => {
    if (typeof db_exportOrganizationsPackage === 'function') {
      return db_exportOrganizationsPackage(templateId)
    } else {
      console.log('not support');
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
  };

  const exportBatch = batchId => {
    if (typeof db_exportPackage === 'function') {
      return db_exportPackage(0, batchId)
    } else {
      console.log('not support');
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
  };

  const exportAir = airId => {
    if (typeof db_exportPackage === 'function') {
      return db_exportPackage(1, airId)
    } else {
      console.log('not support');
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
  };

  const importTemplate = fileContent => {
    // read the excel file
    const r = JSON.parse(fileContent);
    Insert('templates', r.template);
    r.types.forEach(type => {
      Insert('types', type);
    });
    r.codes.forEach(code => {
      Insert('codes', code);
    });
    r.schemas.forEach(schema => {
      Insert('schemas', schema);
    });
    r.lists.forEach(list => {
      Insert('lists', list);
    });
    r.supportConfigs.forEach(supportConfig => {
      Insert('support_configs', supportConfig);
    });
    // write to db
    // console.log(fileContent)
    return 0;
  };

  const importOrganizations = fileContent => {
    if (typeof db_executeSql === 'function') {
      db_executeSql('DELETE FROM "组织";');
      db_executeSql('DELETE FROM "用户";');
    } else {
      const os = Select('组织', '', '');
      os.forEach(o => Remove('组织', o.id));
      const us = Select('用户', '', '');
      us.forEach(u => Remove('用户', u.id));
    }
    // read the excel file
    const r = JSON.parse(fileContent);
    r.organizations.forEach(organization => {
      Insert('组织', organization);
    });
    r.users.forEach(user => {
      Insert('用户', user);
    });
    // write to db
    // console.log(fileContent)
    return 0;
  };

  const importBatch = (fileContent, stateId) => {
    if (typeof db_exportPackage === 'function') {
      return db_importPackage(fileContent, stateId)
    } else {
      console.log('not support');
    }
    // // read the excel file
    // const r = JSON.parse(fileContent)
    // r['状态id'] = stateId
    // // write to db
    // console.log(fileContent, stateId)
  };

  const importAir = (fileContent, organizationId) => {
    if (typeof db_exportPackage === 'function') {
      return db_importPackage(fileContent, organizationId)
    } else {
      console.log('not support');
    }
    // // read the excel file
    // // write to db
    // console.log(fileContent, organizationId)
  };

  const columnIncreament = col => {
    const firstLetter = 'A'.charCodeAt(0);
    const lastLetter = 'Z'.charCodeAt(0);
    const colCodes = [];
    for (let i = 0; i < col.length; ++i) {
      colCodes.push(col.charCodeAt(i));
    }
    let widen = false;
    for (let i = colCodes.length - 1; i >= 0; --i) {
      if (colCodes[i] === lastLetter) {
        colCodes[i] = firstLetter;
      } else {
        ++colCodes[i];
        break
      }
      if (i === 0) {
        widen = true;
      }
    }
    return widen ? String.fromCharCode(firstLetter, ...colCodes) : String.fromCharCode(...colCodes)
  };

  const importConfigurations = (fileContent, rootId) => {
    let baselineId = null;
    const root = selectOne('构型', `id = ${rootId}`);
    if (root) {
      baselineId = root['基线id'];
    }
    const workbook = XLSX.read(fileContent, { type: 'binary' });
    workbook.SheetNames.forEach(sheetName => {
      const range = workbook.Sheets[sheetName]['!ref'];
      if (range.includes(':')) {
        const [first, second] = range.split(':');
        const pattern = /([A-Z]+)([0-9]+)/;
        const [begin, beginColumn, beginRow] = first.match(pattern);
        const [end, endColumn, endRow] = second.match(pattern);
        const lastRow = Number.parseInt(endRow) + 1;
        const lastColumn = columnIncreament(endColumn);
        const idChain = [rootId];
        // 编号	名称	型号	单机定额	装机部位	/*承制单位*/	重要度	状态	主管	版本 备注	扩展1
        let columnIndex = 0;
        for (let row = Number.parseInt(beginRow); row < lastRow; ++row) {
          columnIndex = 0;
          let serialNo;
          let name;
          let model;
          let count;
          let position;
          //let organization
          let importantLevel;
          let state;
          let manager;
          let version;
          let description;
          const extended = {};
          for (let column = beginColumn; column !== lastColumn; column = columnIncreament(column)) {
            const cellName = column + row;
            const contents = workbook.Sheets[sheetName][cellName];
            if (contents && contents.v) {
              const v = contents.v;
              if (v !== '') {
                if (v === '编号') {
                  // the header, skip this row
                  break
                } else {
                  switch (columnIndex) {
                    case 0:
                      serialNo = String(v);
                      break
                    case 1:
                      name = String(v);
                      break
                    case 2:
                      model = String(v);
                      break
                    case 3:
                      count = String(v);
                      break
                    case 4:
                      position = String(v);
                      break
                    // case 5:
                    //   organization = String(v)
                    //   break
                    case 5:
                      importantLevel = String(v);
                      break
                    case 6:
                      state = String(v);
                      break
                    case 7:
                      manager = String(v);
                      break
                    case 8:
                      version = String(v);
                      break
                    case 9:
                      description = String(v);
                      break
                    default:
                      const [propName, propValue] = String(v).split(':');
                      extended[propName] = propValue;
                      break
                  }
                }
                console.log(sheetName + ':' + cellName + ':' + v);
              }
            }
            ++columnIndex;
          }
          if (serialNo && name) {
            // process line
            const serialParts = serialNo.split('.');
            idChain.length = serialParts.length;
            const item = {
              '基线id': baselineId,
              parent_id: idChain[idChain.length - 1],
              '名称': name,
              '型号': model,
              '单机定额': count,
              '装机部位': position,
              // '承制单位id': organization,
              '重要度': importantLevel,
              '状态': state,
              '主管': manager,
              '版本': version,
              '备注': description,
              extended: JSON.stringify(extended)
            };
            idChain.push(Insert('构型', item));
          }
        }
      } else {
        console.log('excel file content error!');
        const contents = workbook.Sheets[sheetName][range];
        if (contents && contents.v) {
          const v = contents.v;
          if (v !== '') {
            console.log(sheetName + ':' + range + ':' + v);
          }
        }
      }
    });
    return 0
  };

  const getAncestors = (templateId, typeNo) => {
    const result = [];
    const lists = Select('lists', `template_id = ${templateId} AND element_type_no = ${typeNo}`, '');
    const allTypes = Select('types', `template_id = ${templateId}`, '');
    result.push(...allTypes.filter(type => {
      let result = false;
      for (const list of lists) {
        if (type.no === list.parent_no) {
          result = true;
          break
        }
      }
      return result
    }));
    const schemas = Select('schemas', `template_id = ${templateId} AND type_no = ${typeNo}`, '');
    const ancestors = [];
    schemas.forEach(schema => {
      ancestors.push(...Select('schemas', `template_id = ${templateId} AND type_no = ${schema.parent_no}`, ''));
    });
    schemas.push(...ancestors);
    result.push(...allTypes.filter(type => {
      let result = false;
      for (const schema of schemas) {
        if (type.no === schema.parent_no) {
          result = true;
          break
        }
      }
      return result
    }));
    return result
  };

  const filterTypes = (templateId, parentTypeNo) => {
    const result = [];
    const type = getTypeByTemplateIdAndNo(templateId, parentTypeNo);
    if (type.type === 2) {
      result.push(...Select('types', `template_id is null AND type = 0`, ''));
      result.push(...Select('types', `template_id = ${templateId} AND type = 1`, ''));
      result.push(...Select('types', `template_id = ${templateId} AND type = 3`, ''));
      const sts = Select('types', `template_id = ${templateId} AND type = 2`, '');
      result.push(...sts.filter(st => {
        let result = true;
        if (type.no === parentTypeNo) {
          result = false;
        } else {
          const ancestors = getAncestors(st);
          for (const t of ancestors) {
            if (t.no === st.no) {
              result = false;
              break
            }
          }
        }
        return result
      }));
    } else if (type.type === 3) {
      result.push(...Select('types', `template_id is null AND type = 0`, ''));
      result.push(...Select('types', `template_id = ${templateId} AND type = 1`, ''));
      result.push(...Select('types', `template_id = ${templateId} AND type = 2`, ''));
    }
    return result
  };

  function downloadFile (content, filename) {
    const a = document.createElement('a');
    const blob = new Blob([content]);
    const url = window.URL.createObjectURL(blob);
    a.href = url;
    a.download = filename;
    a.click();
    window.URL.revokeObjectURL(url);
  }

  const isJSON = str => {
    if (typeof str === 'string') {
      try {
        const obj = JSON.parse(str);
        if (typeof obj == 'object' && obj) {
          return true
        } else {
          return false
        }
      } catch(e) {
        return false
      }
    }
  };

  var DB = {
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
    getBatchExtended,
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
    cloneTemplate,
    cloneBaseLine,
    moveConfigurationNode,
    copyConfigurationNode,
    removeConfigurationNode,
    importConfigurations,
    getTableTypeNo,
    filterTypes,
    tableNos,
    render,
    getTypeByTemplateIdAndNo,
    compare,
    getNewId,
    selectOne,
    exportTemplate,
    exportOrganizations,
    exportBatch,
    exportAir,
    importTemplate,
    importOrganizations,
    importBatch,
    importAir,
    simpleClone,
    extendDeep,
    downloadFile,
    isJSON
  };

  onmessage =  (e) => {
    if (e.data === 'STOP!') {
      close();
    }
    let result = {};
    switch (e.data.type){
        case 'cloneTemplate':
            const templateId = DB.cloneTemplate(e.data.config.cloneTemplateId,e.data.config.name,e.data.config.description);
            result = { templateId: templateId};
            break;
        case 'cloneBaseline':
            const baselineId = DB.cloneBaseLine(e.data.config.oldBaselineId,e.data.config.state,e.data.config.airId,e.data.config.description);
            result = { baselineId: baselineId};
            break;
        case 'test':
            let xxx = typeof db_insert == 'function';
            result = {
                flag :xxx
            };
            break;
    }
    postMessage(result);
  };

})));
