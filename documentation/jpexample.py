import json

from docutils.parsers.rst import Directive, directives
from docutils import nodes


EXAMPLES_COUNT = 0


class jmespath_demo_node(nodes.Element):
    pass


class JPExample(Directive):
    has_content = True
    required_arguments = 1
    optional_arguments = 0
    final_argument_whitespace = True
    option_spec = {
        'rows': directives.unchanged,
        'layout': directives.unchanged,
    }

    default_rows = 10
    default_layout = '1col'


    def run(self):
        expression = self.arguments[0]
        json_data = '\n'.join(self.content)
        self._populate_defaults()
        try:
            # Validate we have valid JSON.
            json.loads(json_data)
        except ValueError as e:
            raise ValueError("Bad JSON in jpexample: %s" % str(e))

        document = self.state.document
        env = document.settings.env
        if self.options['layout'] == '2cols':
            _, filename = env.relfn2path('_static/html/jmespath_2col_demo.html')
        elif self.options['layout'] == '2cols-expand':
            _, filename = env.relfn2path('_static/html/jmespath_2col_expand_demo.html')
        elif self.options['layout'] == '1col':
            _, filename = env.relfn2path('_static/html/jmespath_1col_demo.html')
        else:
            raise ValueError("Unknown layout: %s" % self.options['layout'])

        demo_node = jmespath_demo_node()
        demo_node.jmespath_expr = expression
        demo_node.jmespath_data = json_data
        demo_node.jmespath_options = self.options
        demo_node.jmespath_html_filename = filename
        return [demo_node]

    def _populate_defaults(self):
        if 'rows' not in self.options:
            # Default to the number of '\n' chars in the
            # supplied JSON doc.
            self.options['rows'] = len(self.content)
        if 'layout' not in self.options:
            self.options['layout'] = self.default_layout


def setup(app):
    app.add_directive('jpexample', JPExample)
    app.add_node(jmespath_demo_node, html=(visit_jmespath_demo_node,
                                           depart_jmespath_demo_node))


def visit_jmespath_demo_node(self, node):
    global EXAMPLES_COUNT
    EXAMPLES_COUNT += 1
    idnum = 'jpexample-%s' % EXAMPLES_COUNT
    opts = node.jmespath_options
    with open(node.jmespath_html_filename) as f:
        contents = f.read().format(expression=node.jmespath_expr,
                                   num_rows=opts['rows'],
                                   idnum=idnum,
                                   jmespath_data=node.jmespath_data)
    self.body.append(contents)


def depart_jmespath_demo_node(self, node):
    pass
