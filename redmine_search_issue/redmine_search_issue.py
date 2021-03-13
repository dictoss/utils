#!/usr/bin/python3
#
# search issue for redmine script.
#
#  for debian:
#   # apt-get installl python3-pip
#   # pip3 install python-redmine==2.3.0

import sys
import datetime

from redminelib import Redmine


class MyRedmineApi(object):
    """
    My Redmine API client class
    """
    def __init__(self, url, token, project_name=''):
        self.url = url
        self.token = token
        self._project_name = project_name

        self._rdm = Redmine(url, key=token)
        self._rdm_project = None
        self.limit = 100

    def get_project(self, name=''):
        """
        get project id from project name by call redmine api.
        """
        target_name = ''

        if name:
            target_name = name
        else:
            target_name = self._project_name

        if not target_name:
            return None
        
        try:
            self._rdm_project = self._rdm.project.get(target_name)
        except Exception as e:
            print('EXCEPT: {}'.format(e), file=sys.stderr)
            return None

        return self._rdm_project

    def get_issue(self, id, includes):
       _result = self._rdm.issue.get(id, include=includes)
       return _result

    def filter_issue(self, rdm_project, filters):
       _result = self._rdm.issue.filter(
           project_id=rdm_project.id,
           sort='updated_on:desc', limit=self.limit, offset=0,
           **filters)
       return _result

    def create_issue(self, issue_data, rdm_project):
        _result = issue_data.save()
        return _result

    def extract_issue_data_from_mail(self, mailfile):
        _issue = self._rdm.issue.new()
        _maildata = None

        # parse email
        try:
             _parser = BytesParser()
             with open(mailfile, mode='br') as f:
                _maildata = _parser.parse(f)
        except Exception as e:
            print('EXCEPT 1: {}'.format(e), file=sys.stderr)
            return None

        #
        # set issue
        #
        _issue.project_id = self._rdm_project.identifier

        _issue.subject = self._get_mail_header('Subject', _maildata)
        _issue.description = self._get_mail_body(_maildata)

        _issue.tracker_id = self._get_tracker_id(_maildata)
        _issue.status_id = self._get_status_id(_maildata)
        _issue.priority_id = self._get_priority_id(_maildata)

        # no set
        _issue.start_date = None
        _issue.due_date = None
        _issue.assigned_to_id = None
        _issue.parent_issue_id = None
        _issue.watcher_user_ids = []
        _issue.estimated_hours = None
        _issue.done_ratio = None
        _issue.uploads = []

        # at your own set.
        _issue.custom_fields = []

        return _issue

    def _get_tracker_id(self, maildata):
        return 1

    def _get_status_id(self, maildata):
        return 1

    def _get_priority_id(self, maildata):
        return 1

    def _get_mail_header(self, key, maildata):
        _s = ''
        _h = maildata.get(key)
        _t = decode_header(_h)

        for t in _t:
            _charset = t[1]

            if type(t[0]) is bytes:
                if _charset:
                    _s += t[0].decode(_charset)
                else:
                    _s += t[0].decode()
            elif type(t[0]) is str:
                _s += t[0]
            else:
                pass

        return _s

    def _get_mail_attachedfiles(self, maildata, exclude_html=True):
        """
        get mail attached files
        return [dict{'filename': str, data: bytes}, ...]
        """
        _files = []

        try:
            if maildata.is_multipart:
                for p in maildata.walk():
                    _file = None

                    _filename = p.get_filename()
                    if _filename is None:
                        continue

                    _c_type = p.get_content_type()
                    if (_c_type == 'text/html') and (exclude_html is True):
                        continue

                    _payload = p.get_payload(decode=True)
                    if _payload:
                        _file = {'filename': _filename, 'data': _payload}
                        _files.append(_file)
        except Exception as e:
           print('EXCEPT 1: {}'.format(e), file=sys.stderr)
           return None

        return _files

    def _get_mail_body(self, maildata):
        """
        get mail body
        """
        _s = ''

        try:
            if maildata.is_multipart:
                for p in maildata.walk():
                    _c_type = p.get_content_type()
                    if _c_type == 'text/html':
                        continue
                    elif _c_type == 'text/plain':
                        #print('this part is text/plain')
                        pass

                    _payload = p.get_payload(decode=True)
                    _charset = p.get_content_charset()
                    _filename = p.get_filename()

                    if _payload and (_filename is None):
                        if _charset:
                            _s += _payload.decode(_charset)
                        else:
                            _s += _payload.decode()
                    else:
                        _s += ""

            else:
                _c_type = maildata.get_content_type()
                _charset = maildata.get_content_charset()
                _payload = maildata.get_payload(decode=True)

                if _payload:
                    if _charset:
                      _s = _payload.decode(_charset)
                    else:
                       _s = _payload.decode()
                else:
                    _s = ""
        except Exception as e:
           print('EXCEPT 1: {}'.format(e), file=sys.stderr)
           return None

        return _s


def main(redmine_url, redmine_token, redmine_project_name):

    _redmine = MyRedmineApi(redmine_url, redmine_token, redmine_project_name)

    # convert project id from name
    try:
        _proj = _redmine.get_project()
    except Exception as e:
        print('EXCEPT: {}'.format(e), file=sys.stderr)
        return 1

    # search issues.
    _params = {'updated_on': '>=2020-05-01T00:00:00Z'}
    _issue_list = _redmine.filter_issue(_proj, _params)
    print(_issue_list)

    for i in _issue_list:
        print('-----')
        print('{}: {}'.format(i.id, i.subject))

        # get issues include journals.
        _issue = _redmine.get_issue(i.id, ['journals', 'changesets'])
        print('  {} : {}'.format(  _issue.created_on, _issue.updated_on))

        if _issue.created_on == _issue.updated_on:
            # new issue without journals
            print('    new issue!')
        else:
            # print(_issue.__dict__)
            # print(_issue.journals)

            for j in _issue.journals:
                # print(j.__dict__)
                print('    {}: {} : {}'.format(j.id, j.created_on, j.notes))

    return 0


def usage():
    s = """This script search issue for redmine.

usage: {} [redmine_url] [redmine_token]
""".format(sys.argv[0])
    print(s, file=sys.stderr)


if __name__ == '__main__':
   ret = 0

   if len(sys.argv) < 4:
       usage()
       sys.exit(4)
   else:
       _redmine_url = sys.argv[1]
       _redmine_token = sys.argv[2]
       _redmine_project_name = sys.argv[3]

       ret = main(_redmine_url, _redmine_token,
           _redmine_project_name)

       sys.exit(ret)

