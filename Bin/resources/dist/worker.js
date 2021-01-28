'use strict';
import DB from '/lib/Db'

self.onmessage = function (e) {
  if (e.data === 'STOP!') {
    self.close();
  }
  let result = {}
  switch (e.data.type){
      case 'cloneTemplate':
          const templateId = DB.cloneTemplate(e.data.config.cloneTemplateId,e.data.config.name,e.data.config.description)
          result = { templateId: templateId}
        break;
  }
  self.postMessage(result);
};
