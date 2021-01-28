// id, name, value
const properties = []
// id, name, state, description
const templates = []
// id, template_id, no, name, type, description
const types = []
// id, template_id, length, precision, min, max
const restrictions = []
// id, name, description
const unitTypes = []
// id, type_id, name, symbol, description
const units = []
// id, template_id, parent_id, type, no, name, type_no, restriction_id, unit_id
const infoConfigs = []
// id, template_id, parent_no, no, value, description
const codes = []
// id, template_id, parent_no, no, name, title, is_hidden, type_no, is_required, restriction_id, unit_id, description
const schemas = []
// id, template_id, parent_no, element_type_no, min, max, group_by, description
const lists = []
// id, object_type, object_id, previous_version_id
const baselines = []
// id, info_config_id, baseline_id, parent_id, ref_id, serial_no, data
const dataCollection = []
// id, type, name, ext, content, size, digest
const files = []

// id, parent_id, no, name, type, address, telephone, contact, manager_id, description
const organizations = []
// id, login_name, name, sex, birthday, organization_id, address, telephone, email, website, password, role, description
const users = []
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
