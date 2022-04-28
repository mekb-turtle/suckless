void
grid(Monitor *m) {
	unsigned int i, n, cx, cy, cw, ch, aw, ah, cols, rows;
	Client *c;

	for(n = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next))
		n++;

	/* grid dimensions */
	for(rows = 0; rows <= n/2; rows++)
		if(rows*rows >= n)
			break;
	cols = (rows && (rows - 1) * rows >= n) ? rows - 1 : rows;

	/* window geoms (cell height/width) */
	ch = (m->wh - gappx) / (rows ? rows : 1);
	cw = (m->ww - gappx) / (cols ? cols : 1);
	for(i = 0, c = nexttiled(m->clients); c; c = nexttiled(c->next)) {
		cx = m->wx - gappx + (i / rows) * cw;
		cy = m->wy - gappx + (i % rows) * ch;
		/* adjust height/width of last row/column's windows */
		ah = ((i + 1) % rows == 0) ? m->wh - gappx - ch * rows : 0;
		aw = (i >= rows * (cols - 1)) ? m->ww - gappx - cw * cols : 0;
		resize(c, cx + gappx * 2, cy + gappx * 2, cw - 2 * c->bw + aw - gappx, ch - 2 * c->bw + ah - gappx, False);
		i++;
	}
}
